#ifndef MSERVER_H
#define MSERVER_H

#include <netinet/in.h>
#include <map>
#include <string>
#include <vector>
#include "StructAttribute.h"
#include "UpdateInfo.h"

//! Class that creates the server socket and listen the client
class MServer
{

public:
    MServer(int PORT, int size);
    ~MServer();

    void request(sockaddr_in address, int serverSocket);
    void readSocket(char *bufferOut, sockaddr_in address, int serverSocket, int &newSocket);

    //methods called by interpreter
    void enter_scope();

    void exit_scope();

    int declaration(UpdateInfo declarationInfo);

    int reference_declaration(UpdateInfo declarationInfo);

    int update_value(UpdateInfo declarationInfo);

    void enter_struct();

    int exit_struct(std::string id);

    std::string getInfo(std::string id); //send the variable with type, value, and address info packaged

private:
    //! points to the start of the memory, is char so that getting to an address through arithmetic is easy
    char *serverMemory;

    //! The keys hold the identifier of stored variables and the int values are their distance from serverMemory's address
    std::map<std::string, int> varAddresses;

    //! The keys hold the identifier of stored variables and the string values are their type, reference types are stored as "reference<[type]>"
    std::map<std::string, std::string> varTypes;

    //! The keys are the names of structs previously defined and the the values are StructAttributes which hold the info to build the structs attributes
    std::map<std::string, std::vector<StructAttribute>> structDefinitions;

    //! The counter keeping track of which scope the program is in at the moment
    int scopeLevel;

    //! This flag signals to the declaration method whether the declarations are part of a struct definition or if they are to be stored in memory
    bool insideStruct;

    //! This vector holds the declarations made for a struct being defined until it gets finished
    std::vector<StructAttribute> currentStruct;

    //! This vector holds a counter for each scope level as to how many structs have been defined so that they can be undefined when exiting scopes
    std::vector<int> structDefsCounter;

    //! Vector used to keep track of the insertion order of the structDefinitions map in order to get rid of them when exiting a scope
    std::vector<std::string> structDefinitionsOrder;

    //! This vector holds a counter for each variable that has been declared (including structs and their attributes) so that when exiting a scope they can be freed from memory
    std::vector<int> declarationsCounter;

    std::string getLastVariable();

    int getAvailableAddress();

    std::string extract_refType(std::string reference);

    void updateStructAddresses(std::string structName, std::string newStructName);
};

#endif // MSERVER_H
