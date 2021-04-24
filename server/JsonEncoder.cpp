#include "JsonEncoder.h"
#include <nlohmann/json.hpp>
#include "string"

using namespace std;
using json = nlohmann::json;

JsonEncoder::JsonEncoder() {
}

string JsonEncoder::encode(UpdateInfo data) {
    this->jFile["dataName"] = data.getDataName();
    //this->jFile["dataValue"] = data.getDataValue();
    this->jFile["dataType"] = data.getDataType();
    this->jFile["dataCount"] = data.getDataCount();

    string jString = jFile.dump();
    return jString;
}