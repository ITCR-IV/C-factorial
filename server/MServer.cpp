#include "MServer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "RequestConstants.h"

using namespace std;

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

/*!
 * \brief accepts and reads all the incoming messages from the client
 *
 * \param address indicates de IP address
 * \param serverSocket indicates the serverSocket
 */
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
                printf("Entering scope");
                break;
            case EXITSCOPE:
                printf("Exiting scope");
                break;
            case DECLARE:
                printf("Getting ready for a declaration");
                break;
            case DECLAREREF:
                printf("Okay I'm not doing any others...");
                break;
            case CHANGEVALUE:
                break;
            case ENTERSTRUCT:
                break;
            case EXITSTRUCT:
                break;
            case REQUESTMEMORYSTATE:
                break;
            default:
                printf("Undefined request '%d'", req);
            }
            /////////////////////////// Switch end
        }
        else
        {
            cout << "Couldn't understand request: \"" << buffer << "\". Make sure to send a single integer to indicate the type of request being made.";
        }
        send(newSocket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
    }
}

void MServer::enter_scope() {}

void MServer::exit_scope() {}

void MServer::declaration(string type, string id, string assignType /* = ""*/, string value /* = ""*/) {}

void MServer::reference_declaration(string ptrType, string id, string assignType /* = ""*/, string value /* = ""*/) {}

void MServer::update_value(string id, string assignType, string value) {}

void MServer::enter_struct() {}

void MServer::exit_struct(string id) {}

string getInfo(string id) //send a variable with type, value, and address info packaged, if it doesn't exist send it with empty fields
{
    return "aa";
}