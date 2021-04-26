#ifndef JSONDECODER_H
#define JSONDECODER_H

#include <nlohmann/json.hpp>
#include "UpdateInfo.h"
#include "string"

using namespace std;
using json = nlohmann::json;

//! Class that decode the string form of the Json
class JsonDecoder
{

private:
    //! String form of the Json
    string jsonString;

    //! Json file
    json jFile;

public:
    //! Constructor of the JsonDecoder Class
    JsonDecoder(string jsonString);

    UpdateInfo decode();
};

#endif //JSONDECODER_H
