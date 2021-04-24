#ifndef C_FACTORIAL_JSONENCODER_H
#define C_FACTORIAL_JSONENCODER_H

#include <nlohmann/json.hpp>
#include "UpdateInfo.h"
using json = nlohmann::json;

class JsonEncoder {
private:
    json jFile;
public:
    JsonEncoder();
    encode(UpdateInfo data);
};


#endif
