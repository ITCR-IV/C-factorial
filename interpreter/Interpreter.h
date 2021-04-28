#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include "ServerManager.h"
#include "Token.h"

using namespace std;

class Interpreter
{

private:
    //socket attribute and stuff
    ServerManager *manager;

    void error(const string extraDetails);

public:
    //!struct RuntimeException to define the exception thrown while interpreting
    struct RuntimeException : public exception
    {
        RuntimeException(const string &msg) : msg_(msg) {}

        const char *what() const noexcept { return (&msg_[0]); }

    private:
        string msg_;
    };

    //methods to communicate with server and process stuff
    Interpreter();

    void enter_scope();

    void exit_scope();

    void declaration(string type, string id, string assignType = "", string value = "");

    void reference_declaration(string ptrType, string id, string assignType = "", string value = "");

    void update_value(string id, string assignType, string value);

    void enter_struct();

    void exit_struct(string id);

    Token arithmetic(char operation, Token op1, Token op2);

    string getAddr(string id); //make sure a reference isn't being passed, this returns the address of a variable, turn the int value to a hex string for added coolness

    string getValue(string id); //has to handle regular ids AND struct accesses

    string getRefValue(string id); //same as getValue but it has to make sure a reference is being passed and return the value it's pointing to, this is the interpreter equivalente to the grammar's getValue

    string getType(string id);

    void print_call(string msg);

    string extract_refType(string reference);
};

#endif