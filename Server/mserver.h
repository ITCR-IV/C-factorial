#ifndef MSERVER_H
#define MSERVER_H


#include <netinet/in.h>


class mserver
{
public:
    mserver(int PORT, int size);
    void request(sockaddr_in address, int serverSocket);

private:
    void* serverMemory;

};

#endif // MSERVER_H
