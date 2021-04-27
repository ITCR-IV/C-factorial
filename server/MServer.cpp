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
    serverBind = bind(serverSocket, (sockaddr *)&address, sizeof(sockaddr));

    if (serverBind < 0)
    {
        // add to log
        printf("Fail to bind local port\n");
    }

    else
    {
        // add to log
        printf("Successfully bind to local port\nPort number: %d\n", PORT);
    }

    // set socket to an especific port
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    //listen
    serverBind = listen(serverSocket, 1);

    if (serverBind < 0)
    {
        // add to log
        printf("Fail to start listening local port\n");
    }

    else
    {
        // add to log
        printf("Started listening local port\n");
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
        if (strlen(buffer) == 1 && isdigit(buffer[0]))
        {
            int req = buffer[0] - '0';
            //////////////////////////// Here starts switch to evaluate types of requests
            switch (req)
            {
            case ENTERSCOPE:
                printf("Entering scope\n");
                enter_scope();
                break;
            case EXITSCOPE:
                printf("Exiting scope\n");
                exit_scope();
                break;
            case DECLARE:
            {
                printf("Doing a declaration\n");
                readSocket(buffer, address, serverSocket, newSocket);
                int exitStatus;
                exitStatus = declaration(JsonDecoder(buffer).decode());
                std::string info = std::to_string(exitStatus);
                send(newSocket, info.c_str(), info.length(), 0);
                break;
            }
            case DECLAREREF:
            {
                printf("Doing a reference declaration\n");
                readSocket(buffer, address, serverSocket, newSocket);
                int exitStatus;
                exitStatus = reference_declaration(JsonDecoder(buffer).decode());
                std::string info = std::to_string(exitStatus);
                send(newSocket, info.c_str(), info.length(), 0);
                break;
            }
            case CHANGEVALUE:
            {
                printf("Changing an existing value\n");
                readSocket(buffer, address, serverSocket, newSocket);
                int exitStatus;
                exitStatus = update_value(JsonDecoder(buffer).decode());
                std::string info = std::to_string(exitStatus);
                send(newSocket, info.c_str(), info.length(), 0);
                break;
            }
            case ENTERSTRUCT:
                printf("Entering struct\n");
                enter_struct();
                break;
            case EXITSTRUCT:
            {
                printf("Exiting struct\n");
                readSocket(buffer, address, serverSocket, newSocket);
                int exitStatus;
                exitStatus = exit_struct(std::string(buffer));
                std::string info = std::to_string(exitStatus);
                send(newSocket, info.c_str(), info.length(), 0);
                break;
            }
            case REQUESTVARIABLE:
            {
                printf("Fulfilling variable request");
                readSocket(buffer, address, serverSocket, newSocket);
                std::string info;
                info = getInfo(std::string(buffer));
                send(newSocket, info.c_str(), info.length(), 0);
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
                    send(newSocket, info.c_str(), info.length(), 0);
                }

                std::string exitStatus = std::to_string(SUCCESS);
                send(newSocket, info.c_str(), info.length(), 0);
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

    // accept
    newSocket = accept(serverSocket, (sockaddr *)&address, (socklen_t *)&addressLen);

    if (newSocket < 0)
    {
        // add to log
        printf("Fail to accept\n");
    }
    else

    {
        // add to log
        printf("accepted\n");
    }

    read(newSocket, buffer, 1024);

    printf("Received: '%s'\n", buffer);
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
            printf("Attempted to allocate an existing variable as a new one");
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
                printf("Attempted declaration with unrecognized type");
                return ERROR;
            } // todo find a way to check if an existing struct is being assigned to this one and instead of creating a new one in memory just create new variables in the map pointing to same addresses
            //now declare all of the struct attributes and a struct variable pointing to the start of the struct with the size of 1
            this->varAddresses.insert({name, newAddress});
            this->varTypes.insert({name, type});
            std::vector<StructAttribute> definition = this->structDefinitions.at(type);
            std::vector<StructAttribute>::iterator attribute;
            for (attribute = definition.begin(); attribute < definition.end(); attribute++)
            {
                std::string fullName = name + "." + attribute->name;
                if (attribute->type.find('<') == std::string::npos) // it's a reference
                {
                    this->reference_declaration(UpdateInfo(extract_refType(attribute->type), fullName, attribute->defaultValue));
                }
                else // it's anything else
                {
                    this->declaration(UpdateInfo(attribute->type, fullName, attribute->defaultValue));
                }
            }
        }
    }
    declarationsCounter.back()++;
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
        this->currentStruct.push_back(StructAttribute(type, name, value));
    }
    else
    {
        if (this->varAddresses.find(name) != this->varAddresses.end())
        {
            printf("Attempted to allocate an existing variable as a new one");
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
    }
    declarationsCounter.back()++;
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
        printf("Attempted to change the value of a variable that wasn't previously defined");
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
        printf("Attempted to redefine a struct that was previously defined");
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
    type = varTypes.at(id);

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
    else
    {
        //If a struct object is being requested then value == ""
        value = "";
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
    int lastVarAddress = (std::max_element(this->varAddresses.begin(), this->varAddresses.end(), [](const std::pair<std::string, int> &p1, const std::pair<std::string, int> &p2) {
                             return p1.second < p2.second;
                         }))->second;

    int availAddress;

    if (lastVarAddress > 0)
    {
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
    else
    {
        availAddress = 0;
    }

    return availAddress;
}

/*!
 * \brief This method applies a regex to "reference<$type>" strings to extract the $type from it
 * 
 * \param reference a string of format "reference<$type>" where $type is the type it points to 
 * \return returns the type it points to as a string
 */
string MServer::extract_refType(string reference)
{
    string ptrType = reference.substr(reference.find('<') + 1, reference.find('>') - reference.find('<') - 1); //extract the type inside <>
    return ptrType;
}