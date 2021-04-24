#include "UpdateInfo.h"
#include <string>
using namespace std;

UpdateInfo *UpdateInfo::singleton = nullptr;

/*!
 * \brief Constructor for UpdateInfo, should only be called once due to being a singleton
 *
 * \param
 */
UpdateInfo::UpdateInfo() {
    this->dataCount = 0;
    this->dataName;
    //this->dataValue = 0;
    this->dataType;
}

/*!
 * \brief
 *
 * \param
 * \return UpdateInfo* singleton instance
 */
UpdateInfo *UpdateInfo::getInstance()
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

void UpdateInfo::setDataType(string type) {
    this->dataType = type;
}

string UpdateInfo::getDataType() {
    return this->dataType;
}

void UpdateInfo::setDataName(string name) {
    this->dataName = name;
}

string UpdateInfo::getDataName() {
    return this->dataName;
}

void UpdateInfo::setDataCount(int num) {
    this->dataCount = num;
}

int UpdateInfo::getDataCount() {
    return this->dataCount;
}

//void UpdateInfo::setDataValue(T num) {
//    this->dataCount = num;
//}

//T UpdateInfo::getDataValue() {
//    return this->dataValue;
//}


