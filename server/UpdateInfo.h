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
    UpdateInfo();

    static UpdateInfo *singleton;

public:

    //public singleton stuff
    UpdateInfo(UpdateInfo &other) = delete;   //delete copy constructor (non-cloneable)
    void operator=(const UpdateInfo &) = delete; //delete assignment operator (non-assignable)

    static UpdateInfo *getInstance();

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
