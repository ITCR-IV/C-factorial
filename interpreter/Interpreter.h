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

    void declaration(string type, string id, string value = nullptr);

    void reference_declaration(string ptrType, string id, string value = nullptr);

    void enter_struct();

    void exit_struct();

    int arithmetic(char operation, int op1, int op2);

    void *getAddr(string id);

    string getValue(string id);

    void print_call(string msg);

    string struct_access(string id);
};

#endif