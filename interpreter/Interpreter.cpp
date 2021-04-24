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
<<<<<<< HEAD
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
=======
* \brief  Tell the memory server to allocate a variable defined by the parameters of this method
* 
* \param type the type that is being declared
* \param id the id of the new variable
* \param assignType the type of the value being assigned
* \param value the value being assigned
*/
void Interpreter::declaration(string type, string id, string assignType /*= nullptr*/, string value /*= nullptr*/) {}

/*!
* \brief 
* 
* \param ptrType the type that the reference points to, this is just to check that the type it points to matches the type of reference assigned (the type inside <>, without the <>); Example: int
* \param id the id of the new reference variable
* \param assignType the type of the value being assigned, this is to check that it IS a reference and that the type inside <> matches the ptrType; Example: reference<int>
* \param value the value being assigned
*/
void Interpreter::reference_declaration(string ptrType, string id, string assignType /*= nullptr*/, string value /*= nullptr*/)
{
    id = id + "a";
    return;
}

/*!
 * \brief Like declaration but for stuff inside a struct, so id in this case is a STRUCTACCESS (id.id), it checks that the type of the id and the assignType match and handles references
 * 
 * \param id is a STRUCTACCESS (structid.attributeid), is the attribute that gets modified
 * \param assignType the type of the the value being assigned
 * \param value the value being assigned
 */
void Interpreter::struct_declaration(string id, string assignType, string value) {}

/*!
 * \brief Tell the memory server that the following declarations are for attributes of a struct being constructed
 * 
 */
void Interpreter::enter_struct() {}

/*!
 * \brief Tell the memory server to stop assigning the declarations to the struct type and finishes defining the struct by giving it an id
 * 
 * \param id The identifier of the struct type
 */
void Interpreter::exit_struct(string id) {}

/*!
 * \brief Run the actual arithmetic evaluation
 * 
 * \param operation Either '+', '-', '*', or '/', defines which arithmetic operation to carry out
 * \param op1 Token object with type INT, FLOAT, DOUBLE, LONG and value is it's numerical value
 * \param op2 Token object with type INT, FLOAT, DOUBLE, LONG and value is it's numerical value
 * \return Token where type is either INT, FLOAT, DOUBLE, LONG and value is it's numerical value as a string
 */
Token Interpreter::arithmetic(char operation, Token op1, Token op2) {}

/*!
 * \brief Requests the memory server for the offset address of a variable
 * 
 * \param id id of a variable
 * \return offset address of the variable as string
 */
string Interpreter::getAddr(string id) {}

/*!
 * \brief Returns the value of a given variable, if no such variable hsa been defined throws error
 * 
 * \param id is a variable and can be a plain variable or a struct access refering to a variable inside a struct
 * \return string identifier of the variable
 */
string Interpreter::getValue(string id) {}

/*!
 * \brief do rereference the reference type variables
 * 
 * \param id the id of the reference variable
 * \return string containing the value of the variable the reference points to
 */
string Interpreter::getRefValue(string id) {}

/*!
 * \brief Asks the memory server for the type of a variable
 * 
 * \param id variable id
 * \return string with type, must correspond to one of the types defined in TokenConstants.h
 */
string Interpreter::getType(string id) {}

/*!
 * \brief Prints to Stdout whatever is passed to it
 * 
 * \param msg msg that is to be printed
 */
void Interpreter::print_call(string msg) {}

/*!
 * \brief This method applies a regex to "reference<$type>" strings to extract the $type from it
 * 
 * \param reference a string of format "reference<$type>" where $type is the type it points to 
 * \return returns the type it points to as a string
 */
string extract_refType(string reference)
{
    string ptrType = reference.substr(reference.find('<') + 1, reference.find('>') - reference.find('<') - 1); //extract the type inside <>
    return reference;
}
>>>>>>> interpreter
