#include "UpdateInfo.h"
#include <string>
using namespace std;

/*!
 * \brief Construct a new UpdateInfo object
 * 
 * \param type the type of the variable
 * \param name the identifier of the variable
 * \param value the value of the variable
 * \param count the amount of references pointing to it
 */
UpdateInfo::UpdateInfo(string type, string name, string value, int count)
{
    setDataType(type);
    setDataName(name);
    setDataValue(value);
    setDataCount(count);
}

UpdateInfo::UpdateInfo(const UpdateInfo &obj)
{
    setDataType(obj.getDataType());
    setDataName(obj.getDataName());
    setDataValue(obj.getDataValue());
    setDataCount(obj.getDataCount());
}

/*!
 * \brief set the type of data
 *
 * \param type string that identifies the type of data
 */
void UpdateInfo::setDataType(string type)
{
    this->dataType = type;
}

/*!
 * \brief get the type of the data
 *
 * \return returns a string with the type
 */
string UpdateInfo::getDataType() const
{
    return this->dataType;
}

/*!
 * \brief set the name of the data
 *
 * \param name string with the name
 */
void UpdateInfo::setDataName(string name)
{
    this->dataName = name;
}

/*!
 * \brief get the name of the data
 *
 * \return string with the name
 */
string UpdateInfo::getDataName() const
{
    return this->dataName;
}

/*!
 * \brief set the count of the data
 *
 * \param num int with the number of the pointers
 */
void UpdateInfo::setDataCount(int num)
{
    this->dataCount = num;
}

/*!
 * \brief get the counter of the variable
 *
 * \return int with the counter
 */
int UpdateInfo::getDataCount() const
{
    return this->dataCount;
}

/*!
 * \brief set the value of the data
 * 
 * \param value string representation of the value
 */
void UpdateInfo::setDataValue(string value)
{
    this->dataValue = value;
}

/*!
 * \brief get the value of the data
 * 
 * \return string representation of the data
 */
string UpdateInfo::getDataValue() const
{
    return this->dataValue;
}
