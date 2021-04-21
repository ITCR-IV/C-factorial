#ifndef MSERVER_H
#define MSERVER_H


#include <netinet/in.h>


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
    void* serverMemory;

};

#endif // MSERVER_H
