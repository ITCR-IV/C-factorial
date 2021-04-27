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

/*!
 * \brief Create a new socket connection to server in order to be able to send a new message
 * 
 */
void ServerManager::connectSocket()
{
    //Create new socket
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    //Configure environment for address
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(this->PORT);

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

    // Clear buffer
    this->buffer[0] = '\0';
}

void ServerManager::sendRequest(int request)
{
    connectSocket();
    printf("Sending: %d\n", request);

    std::string msg = std::to_string(request);

    send(this->serverSocket, msg.c_str(), msg.length(), 0);
}

void ServerManager::sendJson(std::string jsonStr)
{
    connectSocket();
    printf("Sending: %s\n", jsonStr.c_str());

    std::string msg = jsonStr;

    send(this->serverSocket, msg.c_str(), msg.length(), 0);
}

void ServerManager::listenServer()
{
    read(this->serverSocket, this->buffer, 1024);
    printf("Received: '%s'\n", this->buffer);
}
