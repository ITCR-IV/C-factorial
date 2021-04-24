#include "ServerManager.h"
#include "UpdateInfo.h"

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
}

/*!
 * \brief method to get (or initialize for the first time) the singleton instance
 * 
 * \param PORT where it'll connect with server
 * \return ServerManager* singleton instance
 */
ServerManager *ServerManager::getInstance(int PORT /*= 9999*/) //Default port so it doesn't always need one when classes just want to get the singleton and not initialize it
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if (singleton == nullptr)
    {
        singleton = new ServerManager(PORT);
    }
    return singleton;
}