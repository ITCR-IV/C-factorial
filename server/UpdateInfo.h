#ifndef UPDATEINFO_H
#define UPDATEINFO_H

#include <string>
using namespace std;

//! Class that stores all the info on each line
class UpdateInfo
{

private:

    //! string that indicates de type of variable
    string dataType;

    //! string with the name of the variable
    string dataName;

    //! string that stores the value of the variable
    //string dataValue;

    //! integer with the number of pointers of an specific variable
    int dataCount;

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


#endif //UPDATEINFO_H
