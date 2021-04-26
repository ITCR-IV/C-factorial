#include "JsonDecoder.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/*!
 * \brief Construct a new JsonDecoder object
 *
 * \param string form of the json
 */
JsonDecoder::JsonDecoder(string jsonString) {
    this->jsonString = jsonString;
}
/*!
 * \brief translates the jsonString to a UpdateInfo object
 *
 * \return A UpdateInfo object with the information from the json
 */
UpdateInfo JsonDecoder::decode(){
    //this->jFile = jsonString._json;
}