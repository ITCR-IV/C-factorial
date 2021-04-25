#include "MServer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "iostream"

using namespace std;

MServer::MServer(int PORT, int size)
{
    struct sockaddr_in address;

    int serverBind = 0;
    int opt = 1;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // malloc of the size
    this->serverMemory = static_cast<char *>(malloc(size));

    // configurar el ambiente para el sockaddr structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //assinging the address of local machine
    address.sin_port = htons(PORT);

    //
    serverBind = bind(serverSocket, (sockaddr *)&address, sizeof(sockaddr));

    if (serverBind < 0)
    {
        // add to log
        printf("Fail to bind local port\n");
    }

    else
    {
        // add to log
        printf("Successfully bind to local port\n");
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

    //Interpreting stuff

    this->scopeLevel = 0;
    this->insideStruct = false;
}

void MServer::request(sockaddr_in address, int serverSocket)
{
    int newSocket = 0;
    int addressLen = sizeof(address);
    char buffer[1024] = {0};
    const char *hello = "Hello from server";
    int valread;
    bool listening = true;
    while (listening)
    {
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

        valread = read(newSocket, buffer, 1024);
        printf("%s\n", buffer);
        if (buffer == "off")
        {
            listening = false;
        }
        send(newSocket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
    }
}

void MServer::enter_scope() {}

void MServer::exit_scope() {}

void MServer::declaration(string type, string id, string assignType /* = nullptr*/, string value /* = nullptr*/) {}

void MServer::reference_declaration(string ptrType, string id, string assignType /* = nullptr*/, string value /* = nullptr*/) {}

void MServer::struct_declaration(string id, string assignType, string value) {}

void MServer::enter_struct() {}

void MServer::exit_struct(string id) {}

string getInfo(string id) //send a variable with type, value, and adress info packaged, if it doesn't exist send it with empty fields
{
    return "aa";
}