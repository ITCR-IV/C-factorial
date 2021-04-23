#include "Interpreter.h"
#include <string>
#include "Token.h"

using namespace std;

Interpreter::Interpreter() : manager(ServerManager::getInstance()) {}

//! struct RuntimeException to define the exception thrown while interpreting
struct Interpreter::RuntimeException : public exception
{
    RuntimeException(const string &msg) : msg_(msg) {}

    string getMessage() const { return (msg_); }

private:
    string msg_;
};

/*!
 * \brief The interpreter throws a RuntimeException if an expression is invalid
 * 
 * \param extraDetails Extra information relating to the error
 */
void Interpreter::error(const string extraDetails = "")
{
    string msg = "Semantic error found in line: ";
    string fullMsg = msg + '\n' + extraDetails;

    throw RuntimeException(fullMsg);
}

void Interpreter::enter_scope() {}

void Interpreter::exit_scope() {}

void Interpreter::declaration(string type, string id, string assignValue /*= nullptr*/, string value /*= nullptr*/) {}

void Interpreter::reference_declaration(string ptrType, string id, string assignValue /*= nullptr*/, string value /*= nullptr*/) {}

void Interpreter::enter_struct() {}

void Interpreter::exit_struct(string id) {}

string Interpreter::arithmetic(char operation, string op1, string op2) {}

void *Interpreter::getAddr(string id) {}

string Interpreter::getValue(string id) {}

string Interpreter::getType(string id) {}

void Interpreter::print_call(string msg) {}

string Interpreter::struct_access(string id) {}