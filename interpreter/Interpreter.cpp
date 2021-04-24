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

/*!
 * \brief Tell the memory server to increment the scope stack so that it can assign new variables in the new scope
 * 
 */
void Interpreter::enter_scope() {}

/*!
 * \brief Tell the memory server to descrease scope stack and release all memory pertaining to the exited scope
 * 
 */
void Interpreter::exit_scope() {}

/*!
 * \brief Tell the memory server to allocate a variable defined by the parameters of this method
 * 
 */
void Interpreter::declaration(string type, string id, string assignType /*= nullptr*/, string value /*= nullptr*/) {}

void Interpreter::reference_declaration(string ptrType, string id, string assignType /*= nullptr*/, string value /*= nullptr*/) {}

void Interpreter::struct_declaration(string id, string assignType, string value) {}

void Interpreter::enter_struct() {}

void Interpreter::exit_struct(string id) {}

Token Interpreter::arithmetic(char operation, Token op1, Token op2) {}

string Interpreter::getAddr(string id) {}

string Interpreter::getValue(string id) {}

string Interpreter::getRefValue(string id) {}

string Interpreter::getType(string id) {}

void Interpreter::print_call(string msg) {}

string Interpreter::struct_access(string id) {}