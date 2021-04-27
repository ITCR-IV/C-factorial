#include "ServerManager.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

ServerManager *ServerManager::singleton = nullptr;

/*!
 * \brief Constructor for ServerManager, should only be called once due to being a singleton
 * 
 * \param PORT where it'll connect with server
 */
ServerManager::ServerManager(int PORT)
{
    this->PORT = PORT;
    // connect to server here
    this->serverSocket = 0;
    struct sockaddr_in address;

    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    //Configure environment for sockaddr struct
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    // Connect to the server's socket
    if (connect(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("\nConnection Failed \n");
        return;
    }
}

/*!
 * \brief method to get (or initialize for the first time) the singleton instance
 * 
 * \param PORT where it'll connect with server
 * \return ServerManager* singleton instance
 */
ServerManager *ServerManager::getInstance(int PORT /*= 9999*/) //Default port so it doesn't always need one when classes just want to get the singleton and not initialize it
{
    if (singleton == nullptr)
    {
        singleton = new ServerManager(PORT);
    }
    return singleton;
}

void ServerManager::sendRequest(int request)
{
    int valread;

    char buffer[1024] = {0};

    std::string msg = std::to_string(request);

    send(this->serverSocket, msg.c_str(), msg.length(), 0);
    valread = read(serverSocket, buffer, 1024);
    printf("%s\n", buffer);
}

void ServerManager::sendJson(std::string jsonStr)
{
    int valread;

    char buffer[1024] = {0};

    std::string msg = jsonStr;

    send(this->serverSocket, msg.c_str(), msg.length(), 0);
    valread = read(serverSocket, buffer, 1024);
    printf("%s\n", buffer);
}