#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

//! this class indicates the type and the value of the token
class Token
{
private:
    string type;

    string value;

public:
    Token(string type, string value);

    string getType();

    string getValue();
};

#endif