#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include "ServerManager.h"

using namespace std;

class Interpreter
{

private:
    //socket attribute and stuff
    ServerManager *manager;

    //error stuff
    struct RuntimeException;

    void error(const string extraDetails);

public:
    //methods to communicate with server and process stuff
    Interpreter();

    void enter_scope();

    void exit_scope();

    void declaration(string type, string id, string assignValue = nullptr, string value = nullptr);

    void reference_declaration(string ptrType, string id, string assignValue = nullptr, string value = nullptr);

    void struct_declaration(string id, string assignValue, string value);

    void enter_struct();

    void exit_struct(string id);

    Token arithmetic(char operation, Token op1, Token op2);

    string getAddr(string id); //make sure a reference isn't being passed

    string getValue(string id); //has to handle regular ids AND struct accesses

    string getRefValue(string id); //same as getValue but it has to make sure a reference is being passed and return the value it's pointing to, this is the interpreter equivalente to the grammar's getValue

    string getType(string id);

    void print_call(string msg);

    string struct_access(string id);
};

#endif