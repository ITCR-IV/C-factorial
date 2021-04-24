#ifndef C_FACTORIAL_JSONDECODER_H
#define C_FACTORIAL_JSONDECODER_H

#include <nlohmann/json.hpp>
#include "UpdateInfo.h"
#include "string"

using namespace std;
using json = nlohmann::json;

class JsonDecoder {
private:
    string jsonString;
    json jFile;

public:
    JsonDecoder(string jsonString);
    decode();
};


#endif
