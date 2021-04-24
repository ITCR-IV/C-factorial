#include "UpdateInfo.h"
#include <string>
using namespace std;


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


