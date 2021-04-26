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
 * \brief set the type of data
 *
 * \param type string that identifies the type of data
 */
void UpdateInfo::setDataType(string type) {
    this->dataType = type;
}

/*!
 * \brief get the type of the data
 *
 * \return returns a string with the type
 */
string UpdateInfo::getDataType() {
    return this->dataType;
}

/*!
 * \brief set the name of the data
 *
 * \param name string with the name
 */
void UpdateInfo::setDataName(string name) {
    this->dataName = name;
}

/*!
 * \brief get the name of the data
 *
 * \return string with the name
 */
string UpdateInfo::getDataName() {
    return this->dataName;
}

/*!
 * \brief set the count of the data
 *
 * \param num int with the number of the pointers
 */
void UpdateInfo::setDataCount(int num) {
    this->dataCount = num;
}

/*!
 * \brief get the counter of the variable
 *
 * \return int with the counter
 */
int UpdateInfo::getDataCount() {
    return this->dataCount;
}

/*!
 * \brief set the value of the data
 *
 * \param
 */
//void UpdateInfo::setDataValue(T num) {
//    this->dataCount = num;
//}

/*!
 * \brief get the value of the data
 *
 * \param
 * \return
 */
//T UpdateInfo::getDataValue() {
//    return this->dataValue;
//}


