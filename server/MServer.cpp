#include "MServer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "RequestConstants.h"
#include "JsonDecoder.h"
#include "JsonEncoder.h"
#include "TypeConstants.h"
#include "StructAttribute.h"
#include <chrono>
#include <thread>
#include "Logger.h"

/*!
 * \brief Construct a new MServer object, and configures the socket
 *
 * \param PORT int that indicates de port number
 * \param size int that indicates how much memory the server is going to reserve
 */
MServer::MServer(int PORT, int size)
{
    //Interpreting stuff

    this->scopeLevel = 0;
    this->insideStruct = false;

    // malloc of the size
    this->serverMemory = static_cast<char *>(malloc(size));

    //Socket stuff

    struct sockaddr_in address;

    int serverBind = 0;
    int opt = 1;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // configurar el ambiente para el sockaddr structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //assinging the address of local machine
    address.sin_port = htons(PORT);

    // binding to port
    while (true)
    {
        serverBind = bind(serverSocket, (sockaddr *)&address, sizeof(sockaddr));

        if (serverBind < 0)
        {
            Logger::EnableFileOutput();
            Logger::Error("Error al hacer bind al puerto local [MServer]");
            printf("Port number: %d\nFailed to bind to local port\n", PORT);
            PORT--;
            address.sin_port = htons(PORT);
        }

        else
        {
            Logger::EnableFileOutput();
            Logger::Info("Bind al puerto local con exito [MServer]");
            printf("Port number: %d\nSuccessfully bind to local port\n", PORT);
            break;
        }
    }

    // set socket to an especific port
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    //listen
    serverBind = listen(serverSocket, 1);

    if (serverBind < 0)
    {
        Logger::EnableFileOutput();
        Logger::Error("Fallo al iniciar la escucha del puerto [MServer]");
    }

    else
    {
        Logger::EnableFileOutput();
        Logger::Info("Escuchando el puerto local[MServer]");
    }

    this->request(address, serverSocket);
}

//! Destroy the MServer::MServer object, the destructor is defined to make sure that the memory allocated with the initial malloc is freed
MServer::~MServer()
{
    free(this->serverMemory);
}

/*!
 * \brief accepts and reads all the incoming messages from the client
 *
 * \param address indicates de IP address
 * \param serverSocket indicates the serverSocket
 */
void MServer::request(sockaddr_in address, int serverSocket)
{
    int newSocket = 0;
    char buffer[1024] = {0};
    bool listening = true;
    while (listening)
    {
        readSocket(buffer, address, serverSocket, newSocket);

        if (strcmp(buffer, "OFF") == 0)
        {
            listening = false;
            return;
        }
        if (isInt(buffer) && atoi(buffer) < REQUESTUPPERBOUND)
        {
            int req = atoi(buffer);
            //////////////////////////// Here starts switch to evaluate types of requests
            switch (req)
            {
            case ENTERSCOPE:
                printf("Entering scope\n");
                enter_scope();

                Logger::EnableFileOutput();
                Logger::Info("Entrando al scope[MServer]");
                break;
            case EXITSCOPE:
                printf("Exiting scope\n");
                exit_scope();

                Logger::EnableFileOutput();
                Logger::Info("Saliendo al scope[MServer]");
                break;
            case DECLARE:
            {
                printf("Doing a declaration\n");
                readSocket(buffer, address, serverSocket, newSocket);
                int exitStatus;
                exitStatus = declaration(JsonDecoder(buffer).decode());
                std::string info = std::to_string(exitStatus);
                send(newSocket, info.c_str(), info.length() + 1, 0);

                Logger::EnableFileOutput();
                Logger::Info("Haciendo una declaracion [MServer]");
                break;
            }
            case DECLAREREF:
            {
                printf("Doing a reference declaration\n");
                readSocket(buffer, address, serverSocket, newSocket);
                int exitStatus;
                exitStatus = reference_declaration(JsonDecoder(buffer).decode());
                std::string info = std::to_string(exitStatus);
                send(newSocket, info.c_str(), info.length() + 1, 0);

                Logger::EnableFileOutput();
                Logger::Info("Haciendo una declaracion por referencia[MServer]");
                break;
            }
            case CHANGEVALUE:
            {
                printf("Changing an existing value\n");
                readSocket(buffer, address, serverSocket, newSocket);
                int exitStatus;
                exitStatus = update_value(JsonDecoder(buffer).decode());
                std::string info = std::to_string(exitStatus);
                send(newSocket, info.c_str(), info.length() + 1, 0);

                Logger::EnableFileOutput();
                Logger::Info("Cambiando una variable existente [MServer]");
                break;
            }
            case ENTERSTRUCT:
                printf("Entering struct\n");
                enter_struct();

                Logger::EnableFileOutput();
                Logger::Info("Entrando a un struct[MServer]");
                break;
            case EXITSTRUCT:
            {
                printf("Exiting struct\n");
                readSocket(buffer, address, serverSocket, newSocket);
                int exitStatus;
                exitStatus = exit_struct(std::string(buffer));
                std::string info = std::to_string(exitStatus);
                send(newSocket, info.c_str(), info.length() + 1, 0);

                Logger::EnableFileOutput();
                Logger::Info("Saliendo a un struct[MServer]");
                break;
            }
            case REQUESTVARIABLE:
            {
                printf("Fulfilling variable request\n");
                readSocket(buffer, address, serverSocket, newSocket);
                std::string info;
                info = getInfo(std::string(buffer));
                send(newSocket, info.c_str(), info.length() + 1, 0);
                break;
            }
            case REQUESTMEMORYSTATE:
            {
                printf("Sending memory state...\n");

                std::vector<std::pair<std::string, int>> addressVector; //we copy the map values into a vector so that we may then sort the vector and send the variables in address order

                std::string info;
                map<std::string, int>::iterator p;

                for (p = this->varAddresses.begin(); p != this->varAddresses.end(); p++)
                {
                    addressVector.push_back(std::make_pair(p->first, p->second));
                }

                sort(addressVector.begin(), addressVector.end(), [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
                    return a.second < b.second;
                }); // sort the vector by the values of it's keys in ascending order (from low to high) so we get the keys in order of their address

                std::vector<std::pair<std::string, int>>::iterator vp;
                for (vp = addressVector.begin(); vp != addressVector.end(); vp++)
                {
                    info = getInfo(vp->first);
                    send(newSocket, info.c_str(), info.length() + 1, 0);
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }

                std::string exitStatus = std::to_string(SUCCESS);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                send(newSocket, exitStatus.c_str(), exitStatus.length() + 1, 0);

                Logger::EnableFileOutput();
                Logger::Info("Solicitando estado de la memoria [MServer]");
                break;
            }
            case FLUSH:
                printf("Flushing memory state\n");
                //Reset the memory server state
                flushMemory();
                break;
            case REQUESTBYADDRESS:
            {
                printf("Fulfilling variable by address request\n");
                //Receive an address and send back an updateInfo packaged variable
                readSocket(buffer, address, serverSocket, newSocket);
                std::string info;
                info = getInfoByAddr(std::string(buffer));
                send(newSocket, info.c_str(), info.length() + 1, 0);
                break;
            }
            default:
                printf("Undefined request '%d'\n", req);
            }
            /////////////////////////// Switch end
        }
        else
        {
            std::cout << "Couldn't understand request: \"" << buffer << "\". Make sure to send a single integer to indicate the type of request being made.";
        }
        //send(newSocket, hello, strlen(hello), 0);
    }
}

/*!
 * \brief Wait and read a message incoming in the server socket
 * 
 * \param buffer a char* buffer to store the contents read
 * \param address indicates de IP address
 * \param serverSocket indicates the serverSocket
 * \param newSocket the socket that will be accepted and read
 */
void MServer::readSocket(char *buffer, sockaddr_in address, int serverSocket, int &newSocket)
{
    int addressLen = sizeof(address);
    memset(buffer, 0, 1024); // Clear buffer

    // accept
    newSocket = accept(serverSocket, (sockaddr *)&address, (socklen_t *)&addressLen);

    if (newSocket < 0)
    {
        Logger::EnableFileOutput();
        Logger::Error("Fallo al aceptar [MServer]");
    }
    else

    {
        Logger::EnableFileOutput();
        Logger::Info("Aceptado [MServer]");
    }

    read(newSocket, buffer, 1024);
}

//! Increment the scope counte and add a new 0 entry to the structDefsCounter and declarationsCounter
void MServer::enter_scope()
{
    this->scopeLevel++;
    this->structDefsCounter.push_back(0);
    this->declarationsCounter.push_back(0);
}

//! Decrease the scope counter and release the amount of variables equal to the last entry in declarationsCounter while popping it, also removes the last entry in structDefsCounter and removes the amount of struct definitions equal to the counter
void MServer::exit_scope()
{
    int declarationNum = this->declarationsCounter.back();
    int structDefNum = this->structDefsCounter.back();
    this->declarationsCounter.pop_back();
    this->structDefsCounter.pop_back();
    for (declarationNum; declarationNum > 0; declarationNum--)
    {
        std::string last = getLastVariable();
        this->varAddresses.erase(last);
        this->varTypes.erase(last);
    }
    for (structDefNum; structDefNum > 0; structDefNum--)
    {
        std::string last = this->structDefinitionsOrder.back();
        this->structDefinitionsOrder.pop_back();
        this->structDefinitions.erase(last);
    }
    this->scopeLevel--;
}

/*!
 * \brief Add to memory the variable described by declarationInfo and increase the last entry in declarationsCounter, if inside struct then they don't get stored but assigned to the currentStruct vector as StructAttribute objects and no counter is increased. Lastly, if a defined struct object is being declared the declarationCounter must be increased by as many attributes the struct has +1 (because a variable is stored for the struct itself + its attributes). For declaring a variable without a value, pass the value field as an empty string ("").
 * 
 * \param declarationInfo UpdateInfo where the counter value is ignored and the rest is used to store a new variable in memory
 * \return int -1 for error (most likely the variable is already stored) and 0 for success
 */
int MServer::declaration(UpdateInfo declarationInfo)
{
    std::string type = declarationInfo.getDataType();
    std::string name = declarationInfo.getDataName();
    std::string value = declarationInfo.getDataValue();

    if (insideStruct)
    {
        //add to vector a StructAttribute
        this->currentStruct.push_back(StructAttribute(type, name, value));
    }
    else
    {
        if (this->varAddresses.find(name) != this->varAddresses.end())
        {
            printf("Attempted to allocate an existing variable as a new one\n");
            return ERROR;
        }

        int newAddress = getAvailableAddress(); //Get the first available address

        if (type == CHAR)
        {
            if (value != "")
            {
                char *c = this->serverMemory + newAddress;
                *c = value[0];
            }
            this->varAddresses.insert({name, newAddress});
            this->varTypes.insert({name, type});
        }
        else if (type == INT)
        {
            if (value != "")
            {
                int *i = reinterpret_cast<int *>(this->serverMemory + newAddress);
                *i = stoi(value);
            }
            this->varAddresses.insert({name, newAddress});
            this->varTypes.insert({name, type});
        }
        else if (type == FLOAT)
        {
            if (value != "")
            {
                float *f = reinterpret_cast<float *>(this->serverMemory + newAddress);
                *f = stof(value);
            }
            this->varAddresses.insert({name, newAddress});
            this->varTypes.insert({name, type});
        }
        else if (type == DOUBLE)
        {
            if (value != "")
            {
                double *i = reinterpret_cast<double *>(this->serverMemory + newAddress);
                *i = stod(value);
            }
            this->varAddresses.insert({name, newAddress});
            this->varTypes.insert({name, type});
        }
        else if (type == LONG)
        {
            if (value != "")
            {
                long *i = reinterpret_cast<long *>(this->serverMemory + newAddress);
                *i = stol(value);
            }
            this->varAddresses.insert({name, newAddress});
            this->varTypes.insert({name, type});
        }
        else
        { // Declaring a defined struct
            // Note about structs: their size by themselves is just 1 byte pointing to the struct start, the total size would be the size of their attributes + 1
            if (this->structDefinitions.find(type) == this->structDefinitions.end())
            {
                printf("Attempted declaration with unrecognized type\n");
                return ERROR;
            }
            // todo find a way to check if an existing struct is being assigned to this one and instead of creating a new one in memory just create new variables in the map pointing to same addresses
            // for the todo: the value field for a defined struct would be it's address
            // for the todo: because of the way popping variables works when going out of scope, i'd either need to implement a vector to keep track of the order in which i assign new variables or possibly lie in the ram live view
            //now declare all of the struct attributes and a struct variable pointing to the start of the struct with the size of 1
            this->varAddresses.insert({name, newAddress});
            this->varTypes.insert({name, type});
            std::vector<StructAttribute> definition = this->structDefinitions.at(type);
            std::vector<StructAttribute>::iterator attribute;
            for (attribute = definition.begin(); attribute < definition.end(); attribute++)
            {
                std::string fullName = name + "." + attribute->name;
                if (attribute->type.find('<') != std::string::npos) // it's a reference
                {
                    this->reference_declaration(UpdateInfo(extract_refType(attribute->type), fullName, attribute->defaultValue));
                }
                else // it's anything else
                {
                    this->declaration(UpdateInfo(attribute->type, fullName, attribute->defaultValue));
                }
            }
        }
        declarationsCounter.back()++;
        declarationsOrder.push_back(name);
    }
    return SUCCESS;
}

/*!
 * \brief Add to memory the variable described by declarationInfo and increase the last entry in declarationsCounter, if inside struct then they don't get stored but assigned to the currentStruct vector as StructAttribute objects and no counter is increased. For declaring a variable without a value, pass the value field as an empty string ("").
 * 
 * \param declarationInfo UpdateInfo where the counter value is ignored and the rest is used to store a new variable in memory, the type is the type the reference points to and has "reference<$type>" wrapped around it when stored
 * \return int -1 for error (most likely the variable is already stored) and 0 for success
 */
int MServer::reference_declaration(UpdateInfo declarationInfo)
{
    std::string type = declarationInfo.getDataType();
    std::string name = declarationInfo.getDataName();
    std::string value = declarationInfo.getDataValue();

    if (insideStruct)
    {
        //add to vector a StructAttribute
        std::string refType = "reference<" + type + ">";
        this->currentStruct.push_back(StructAttribute(refType, name, value));
    }
    else
    {
        if (this->varAddresses.find(name) != this->varAddresses.end())
        {
            printf("Attempted to allocate an existing variable as a new one\n");
            return ERROR;
        }
        int newAddress = getAvailableAddress();

        if (value != "")
        {
            int *i = reinterpret_cast<int *>(this->serverMemory + newAddress); //references are stored in memory with int* because addresses are just that
            *i = stoi(value);
        }
        this->varAddresses.insert({name, newAddress});

        std::string refType = "reference<" + type + ">";
        this->varTypes.insert({name, refType});
        declarationsCounter.back()++;
    }
    return SUCCESS;
}

/*!
 * \brief Changes the value of a stored variable
 * 
 * \param declarationInfo the name and value are extracted
 * \return int -1 for error (most likely the variable hasn't been previously defined) and 0 for success
 */
int MServer::update_value(UpdateInfo declarationInfo)
{
    std::string name = declarationInfo.getDataName();
    std::string newValue = declarationInfo.getDataValue();

    if (this->varAddresses.find(name) == this->varAddresses.end())
    {
        printf("Attempted to change the value of a variable that wasn't previously defined\n");
        return ERROR;
    }
    std::string type = this->varTypes.at(name);
    int currentAddress = this->varAddresses.at(name);

    if (type == CHAR)
    {
        char *c = this->serverMemory + currentAddress;
        *c = newValue[0];
    }
    else if (type == INT)
    {
        int *i = reinterpret_cast<int *>(this->serverMemory + currentAddress);
        *i = stoi(newValue);
    }
    else if (type == FLOAT)
    {
        float *f = reinterpret_cast<float *>(this->serverMemory + currentAddress);
        *f = stof(newValue);
    }
    else if (type == DOUBLE)
    {
        double *d = reinterpret_cast<double *>(this->serverMemory + currentAddress);
        *d = stod(newValue);
    }
    else if (type == LONG)
    {
        long *l = reinterpret_cast<long *>(this->serverMemory + currentAddress);
        *l = stol(newValue);
    }
    else
    {
        //If a struct object is being changed then it's addresses need to be updated to whatever object's being assigned addresses
    }

    return SUCCESS;
}

//! Changes the insideStruct flag to true so the next declarations go to the currentStruct vector instead of being stored in memory
void MServer::enter_struct()
{
    this->insideStruct = true;
}

/*!
 * \brief Changes the insideStruct flag to false and assigns the struct name along with the currentStruct vector (which gets flushed) to the structDefinitions map and also adds the id to the structDefinitionsOrder vector, also increments the structDefs counter for the last element so when exiting the scope the definition can be erased
 * 
 * \param id name of the new struct class
 * \return int -1 for error (most likely the struct has been previously defined) and 0 for success
 */
int MServer::exit_struct(std::string id)
{
    if (this->structDefinitions.find(id) != this->structDefinitions.end())
    {
        printf("Attempted to redefine a struct that was previously defined\n");
        return ERROR;
    }
    this->structDefinitions.insert({id, this->currentStruct});
    this->structDefinitionsOrder.push_back(id);
    this->currentStruct.clear();
    this->insideStruct = false;
    this->structDefsCounter.back()++;
    return SUCCESS;
}

/*!
 * \brief Send a variable with type, name, value, address, and counters info packaged as an UpdateInfo json, if it doesn't exist send it with empty fields, structs have an empty value field
 * 
 * \param id name of the variable
 * \return std::string formatted UpdateInfo as json
 */
std::string MServer::getInfo(std::string id)
{
    std::string name = id;
    std::string type;
    std::string value;
    int count;
    int dataAddress;

    // get type
    try
    {
        type = varTypes.at(id);
    }
    catch (std::out_of_range e)
    {
        printf("Variable not present in memory\n");
        return std::to_string(ERROR);
    }

    // get address
    dataAddress = varAddresses.at(id);

    // get value
    if (type == CHAR)
    {
        char *c = this->serverMemory + dataAddress;
        std::string newValue = std::string(1, *c);
        value = newValue;
    }
    else if (type == INT)
    {
        int *i = reinterpret_cast<int *>(this->serverMemory + dataAddress);
        value = std::to_string(*i);
    }
    else if (type == FLOAT)
    {
        float *f = reinterpret_cast<float *>(this->serverMemory + dataAddress);
        value = std::to_string(*f);
    }
    else if (type == DOUBLE)
    {
        double *d = reinterpret_cast<double *>(this->serverMemory + dataAddress);
        value = std::to_string(*d);
    }
    else if (type == LONG)
    {
        long *l = reinterpret_cast<long *>(this->serverMemory + dataAddress);
        value = std::to_string(*l);
    }
    else if (type.find('<') != std::string::npos)
    {
        int *i = reinterpret_cast<int *>(this->serverMemory + dataAddress);
        value = std::to_string(*i);
    }
    else
    {
        //If a struct object is being requested then value == it's address
        value = std::to_string(this->varAddresses.at(id));
    }

    // get count

    map<std::string, int>::iterator p1;
    count = 0;

    //count how many in varAddresses hold this address (only happens with struct stuff)
    for (p1 = this->varAddresses.begin(); p1 != this->varAddresses.end(); p1++)
    {
        if (p1->second == dataAddress)
        {
            count++;
        }
    }

    map<std::string, std::string>::iterator p2;
    //search amongst the reference types the ones pointing to this address
    for (p2 = this->varTypes.begin(); p2 != this->varTypes.end(); p2++)
    {
        if (p2->second.find('<') != std::string::npos) //means we found a reference and now we check if it points to the address
        {
            int *r = reinterpret_cast<int *>(this->serverMemory + varAddresses.at(p2->first)); //get an int pointer to the address where the reference value is stored
            if (*r == dataAddress)
            { // reference does point to the given address
                count++;
            }
        }
    }

    //Encode info as JSON

    UpdateInfo info = UpdateInfo(type, name, value, dataAddress, count);
    JsonEncoder encoder = JsonEncoder(info);

    return encoder.encode();
}

/*!
 * \brief It finds the first variable that's stored in the given address (different struct identifiers can point to the same memory) and calls the getInfo method for the found id and returns it's info
 * 
 * \param address address of the variable requested, the string must contain an integer or the method will fail and return an error code
 * \return std::string formatted UpdateInfo as json
 */
std::string MServer::getInfoByAddr(std::string address)
{
    if (!isInt(address.c_str()))
    {
        printf("Address given to getInfoByAddr() method is not an integer");
        return std::to_string(ERROR);
    }
    int iAddress = std::stoi(address);
    map<std::string, int>::iterator p;
    p = this->varAddresses.begin();
    while (true)
    {
        if (p == this->varAddresses.end())
        { // if address not found return error
            printf("Given address for getInfoByAddr() method is not pointed to by any stored variable");
            return std::to_string(ERROR);
        }
        if (p->second == iAddress)
        { // if address found exit loop
            break;
        }
        p++;
    }
    std::cout << "Found variable: " << p->first << "\nWith address: '" << p->second << "' so, yea...\n";
    return getInfo(p->first);
}

/*!
 * \brief Get the last variable assigned in memory, that is, the variable that has the highest address
 * 
 * \return std::string the variable's id
 */
std::string MServer::getLastVariable()
{
    std::string lastVarId;
    lastVarId = (std::max_element(this->varAddresses.begin(), this->varAddresses.end(), [](const std::pair<std::string, int> &p1, const std::pair<std::string, int> &p2) {
                    return p1.second < p2.second;
                }))->first;
    return lastVarId;
}

/*!
 * \brief Returns the first available address, for this it looks for the last variable in memory and adds to it the amount of bytes its type takes up
 * 
 * \return int first available address in memory for allocating new memory
 */
int MServer::getAvailableAddress()
{
    int availAddress;

    if (varAddresses.empty())
    {
        availAddress = 0;
    }
    else
    {
        int lastVarAddress = (std::max_element(this->varAddresses.begin(), this->varAddresses.end(), [](const std::pair<std::string, int> &p1, const std::pair<std::string, int> &p2) {
                                 return p1.second < p2.second;
                             }))->second;

        std::string lastVarId = getLastVariable();
        std::string type = this->varTypes.at(lastVarId);
        if (type == CHAR)
        {
            availAddress = lastVarAddress + 1;
        }
        else if (type == INT || type == FLOAT || type.find('<') != std::string::npos)
        {
            availAddress = lastVarAddress + 4;
        }
        else if (type == LONG || type == DOUBLE)
        {
            availAddress = lastVarAddress + 8;
        }
        else
        { //The last address is an empty struct
            availAddress = lastVarAddress + 1;
        }
    }

    return availAddress;
}

/*!
 * \brief This method applies a regex to "reference<$type>" strings to extract the $type from it
 * 
 * \param reference a string of format "reference<$type>" where $type is the type it points to 
 * \return returns the type it points to as a string
 */
std::string MServer::extract_refType(std::string reference)
{
    string ptrType = reference.substr(reference.find('<') + 1, reference.find('>') - reference.find('<') - 1); //extract the type inside <>
    return ptrType;
}

/*!
 * \brief Changes all the addresses of a struct type and it's attributes to a new struct type that's assigned
 * 
 * \param structName the name of the struct type variable that is being assigned the new address
 * \param newStructName the name of the struct who's addresses are going to be copied to the structName
 */
void MServer::updateStructAddresses(std::string structName, std::string newStructName)
{
    this->varAddresses.at(structName) = this->varAddresses.at(newStructName);

    std::string structPrefix = structName + ".";
    std::string newStructPrefix = newStructName + ".";

    map<std::string, int>::iterator p;
    for (p = varAddresses.begin(); p != varAddresses.end(); p++)
    {
        if (p->first.find(newStructPrefix) != std::string::npos)
        {
            std::string attributeName = p->first.substr(p->first.find(".") + 1); // the name of the attribute without prefix
            std::string attributeEquivalent = structPrefix + attributeName;
            varAddresses.at(attributeEquivalent) = p->second;
        }
    }
}

void MServer::flushMemory()
{
    // Clear maps
    this->varAddresses.clear();
    this->varTypes.clear();
    this->structDefinitions.clear();

    // Clear vectors
    this->currentStruct.clear();
    this->structDefsCounter.clear();
    this->structDefinitionsOrder.clear();
    this->declarationsCounter.clear();
    this->declarationsOrder.clear();

    // Clear flags/variables
    this->scopeLevel = 0;
    this->insideStruct = false;
}

bool MServer::isInt(const char *numberPtr)
{
    while (*numberPtr != '\0')
    {
        if (!isdigit(*numberPtr))
        {
            return false;
        }
        numberPtr++;
    }
    return true;
}