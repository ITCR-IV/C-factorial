//
// Created by ignacio on 22/4/21.
//

#ifndef C_FACTORIAL_UPDATEINFO_H
#define C_FACTORIAL_UPDATEINFO_H


class UpdateInfo {
    template<typename T>
private:
    string dataType;
    string dataName;
    int dataCount;
    T dataValue;

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

    void setDataType(string type);
    string getDataType();
};


#endif //C_FACTORIAL_UPDATEINFO_H
