//
// Created by ignacio on 22/4/21.
//

#include "UpdateInfo.h"


UpdateInfo *UpdateInfo::singleton = nullptr;

/*!
 * \brief Constructor for UpdateInfo, should only be called once due to being a singleton
 *
 * \param
 */
UpdateInfo::UpdateInfo() {

}

/*!
 * \brief
 *
 * \param
 * \return UpdateInfo* singleton instance
 */
UpdateInfo *UpdateInfo::UpdateInfo()
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if (singleton == nullptr)
    {
        singleton = new UpdateInfo();
    }
    return singleton;
}