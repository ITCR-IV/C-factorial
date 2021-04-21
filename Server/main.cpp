
#include "mserver.h"
#include "iostream"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "iostream"
#define PORT 9999
using namespace std;


int main(int argc, char *argv[])
{
//    mserver Server = mserver(9999, 10000);
//    return 0;
    struct sockaddr_in address;

    int serverBind = 0;

    int newSocket = 0;

    int addressLen = sizeof (address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    int valread;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // malloc of the size
    //serverMemory = malloc(size);


    // configurar el ambiente para el sockaddr structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //assinging the address of local machine
    address.sin_port = htons(PORT);

    //
    serverBind = bind(serverSocket,(sockaddr *) &address, sizeof (sockaddr));
    if (serverBind < 0)
    {
        // add to log
        printf("Fail to bind local port");
    }

    else
    {
        // add to log
        printf("Successfully bind to local port");
    }

    //listen
    serverBind = listen(serverSocket, 1);

    if (serverBind < 0)
    {
        // add to log
        printf("Fail to start listening local port");
    }

    else
    {
        // add to log
        printf("Started listening local port");
    }

    // accept

    newSocket = accept(serverSocket,(sockaddr *) &address, (socklen_t*)&addressLen);

    if (newSocket < 0)
    {
        // add to log
        printf("Fail to accept");
    }
    else

    {
        // add to log
        printf("accepted");
    }
    valread = read( newSocket , buffer, 1024);
    printf("%s\n",buffer );
    send(newSocket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");

}
