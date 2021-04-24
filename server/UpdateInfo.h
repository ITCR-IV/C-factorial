#ifndef C_FACTORIAL_UPDATEINFO_H
#define C_FACTORIAL_UPDATEINFO_H

#include <string>
using namespace std;

class UpdateInfo {
 //   template<typename T>;
private:
    // data that is going to be send
    string dataType;
    string dataName;
    int dataCount;
//    T *dataValue;

public:
    UpdateInfo();
    void setDataType(string type);
    string getDataType();

    void setDataName(string name);
    string getDataName();

    void setDataCount(int num);
    int getDataCount();

//    void setDataValue(T num);
//    T getDataValue();
};


#endif
