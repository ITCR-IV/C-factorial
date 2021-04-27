#include "JsonEncoder.h"
#include <nlohmann/json.hpp>
#include <string>
#include "UpdateInfo.h"

using namespace std;
using json = nlohmann::json;

/*!
 * \brief Construct a new JsonEncoder object
 *
 * \param text The entire code text
 */
JsonEncoder::JsonEncoder(const UpdateInfo &data) : data(data) {}

/*!
 * \brief translates the UpdateInfo object to a jsonString
 *
 * \return A string form of the a json
 */
string JsonEncoder::encode()
{
    this->jFile["dataName"] = this->data.getDataName();
    this->jFile["dataValue"] = this->data.getDataValue();
    this->jFile["dataType"] = this->data.getDataType();
    this->jFile["dataAddress"] = this->data.getDataAddress();
    this->jFile["dataCount"] = this->data.getDataCount();

    string jString = this->jFile.dump();
    return jString;
}