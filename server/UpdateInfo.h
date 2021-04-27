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
    string dataValue;

    //! integer with the number of pointers of an specific variable
    int dataCount;

public:
    UpdateInfo(string type, string name, string value, int count);

    UpdateInfo(const UpdateInfo &obj);

    void setDataType(string type);
    string getDataType() const;

    void setDataName(string name);
    string getDataName() const;

    void setDataCount(int num);
    int getDataCount() const;

    void setDataValue(string value);
    string getDataValue() const;
};

#endif //UPDATEINFO_H
