#include "JsonDecoder.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

JsonDecoder::JsonDecoder(string jsonString) {
    this->jsonString = jsonString;
}

UpdateInfo JsonDecoder::decode(){
    //this->jFile = jsonString._json;
}