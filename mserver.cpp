#include "mserver.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include "iostream"
#define PORT 9999

using namespace std;

mserver::mserver()
{
    struct sockaddr_in address;
    int serverBind = 0;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

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
    } else
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
    } else
    {
        // add to log
        printf("Started listening local port");
    }
}
