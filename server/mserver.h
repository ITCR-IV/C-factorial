#ifndef MSERVER_H
#define MSERVER_H

#include <netinet/in.h>
#include <map>
#include <string>
#include <vector>
#include "StructAttribute.h"

class mserver
{
public:
    mserver(int PORT, int size);
    void request(sockaddr_in address, int serverSocket);
    // receiveClass deserializer(JSON file);
    // Json serializer(receiveClass Object);
    void memoryRequest(int num);
    void memoryDeletion(int num);

private:
    //! points to the start of the memory, is char so that getting to an address through arithmetic is easy
    char *serverMemory;

    //! The keys hold the identifier of stored variables and the int values are their distance from serverMemory's address
    std::map<std::string, int> varAddresses;

    //! The keys hold the identifier of stored variables and the string values are their type, reference types are stored as "reference<[type]>"
    std::map<std::string, std::string> varTypes;

    //! The keys are the names of structs previously defined and the the values are StructAttributes which hold the info to build the structs attributes
    std::map<std::string, std::vector<StructAttribute>> structDefinitions;
};

#endif // MSERVER_H
