#include "JsonDecoder.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

/*!
 * \brief Construct a new JsonDecoder object
 *
 * \param string form of the json
 */
JsonDecoder::JsonDecoder(string jsonString)
{
    this->jsonString = jsonString;
}
/*!
 * \brief translates the jsonString to a UpdateInfo object
 *
 * \return A UpdateInfo object with the information from the json
 */
UpdateInfo JsonDecoder::decode()
{
    this->jFile = json::parse(this->jsonString);
    std::string type = this->jFile["dataType"].get<std::string>();
    std::string name = this->jFile["dataName"].get<std::string>();
    std::string value = this->jFile["dataValue"].get<std::string>();
    int address = stoi(this->jFile["dataAddress"].get<std::string>());
    int count = stoi(this->jFile["dataCount"].get<std::string>());
    UpdateInfo returnInfo = UpdateInfo(type, name, value, address, count);
    return returnInfo;
}