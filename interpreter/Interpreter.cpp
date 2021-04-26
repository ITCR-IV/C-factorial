#include "Interpreter.h"
#include <string>
#include "Token.h"
#include "TokenConstants.h"

using namespace std;

Interpreter::Interpreter() : manager(ServerManager::getInstance()) {}

//! struct RuntimeException to define the exception thrown while interpreting
struct Interpreter::RuntimeException : public exception
{
    RuntimeException(const string &msg) : msg_(msg) {}

    const char *what() const noexcept { return (&msg_[0]); }

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
* \brief  Tell the memory server to allocate a variable defined by the parameters of this method , or if the id is already defined then change it's value
* 
* \param type the type that is being declared
* \param id the id of the new variable
* \param assignType the type of the value being assigned
* \param value the value being assigned
*/
void Interpreter::declaration(string type, string id, string assignType /*= ""*/, string value /*= ""*/) {}

/*!
* \brief Tell the memory server to allocate a variable defined by the parameters of this method, or if the id is already defined then change it's value
* 
* \param ptrType the type that the reference points to, this is just to check that the type it points to matches the type of reference assigned (the type inside <>, without the <>); Example: int
* \param id the id of the new reference variable
* \param assignType the type of the value being assigned, this is to check that it IS a reference and that the type inside <> matches the ptrType; Example: reference<int>
* \param value the value being assigned
*/
void Interpreter::reference_declaration(string ptrType, string id, string assignType /*= ""*/, string value /*= ""*/)
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
Token Interpreter::arithmetic(char operation, Token op1, Token op2)
{
    // The order of casting is Long > Double > Float > int
    // Long before double because there's no bigger data type to handle long floats/doubles, so precision has to be removed
    string num1 = op1.getValue();
    string num2 = op2.getValue();
    if (op1.getType() == LONG || op2.getType() == LONG)
    {
        //strip numbers from their decimals
        num1 = num1.substr(0, num1.find('.'));
        num2 = num2.substr(0, num2.find('.'));
        long factor1 = stol(num1);
        long factor2 = stol(num2);
        long resultValue;
        if (operation == '+')
        {
            resultValue = factor1 + factor2;
        }
        else if (operation == '-')
        {
            resultValue = factor1 - factor2;
        }
        else if (operation == '*')
        {
            resultValue = factor1 * factor2;
        }
        else if (operation == '/')
        {
            resultValue = factor1 / factor2;
        }
        return Token(LONG, to_string(resultValue));
    }
    else if (op1.getType() == DOUBLE || op2.getType() == DOUBLE)
    {
        double factor1 = stod(num1);
        double factor2 = stod(num2);
        double resultValue;
        if (operation == '+')
        {
            resultValue = factor1 + factor2;
        }
        else if (operation == '-')
        {
            resultValue = factor1 - factor2;
        }
        else if (operation == '*')
        {
            resultValue = factor1 * factor2;
        }
        else if (operation == '/')
        {
            resultValue = factor1 / factor2;
        }
        return Token(DOUBLE, to_string(resultValue));
    }
    else if (op1.getType() == FLOAT || op2.getType() == FLOAT)
    {
        float factor1 = stof(num1);
        float factor2 = stof(num2);
        float resultValue;
        if (operation == '+')
        {
            resultValue = factor1 + factor2;
        }
        else if (operation == '-')
        {
            resultValue = factor1 - factor2;
        }
        else if (operation == '*')
        {
            resultValue = factor1 * factor2;
        }
        else if (operation == '/')
        {
            resultValue = factor1 / factor2;
        }
        return Token(FLOAT, to_string(resultValue));
    }
    else
    {
        int factor1 = stoi(num1);
        int factor2 = stoi(num2);
        int resultValue;
        if (operation == '+')
        {
            resultValue = factor1 + factor2;
        }
        else if (operation == '-')
        {
            resultValue = factor1 - factor2;
        }
        else if (operation == '*')
        {
            resultValue = factor1 * factor2;
        }
        else if (operation == '/')
        {
            resultValue = factor1 / factor2;
        }
        return Token(INT, to_string(resultValue));
    }
}

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
void Interpreter::print_call(string msg)
{
    printf(&msg[0]);
}

/*!
 * \brief This method applies a regex to "reference<$type>" strings to extract the $type from it
 * 
 * \param reference a string of format "reference<$type>" where $type is the type it points to 
 * \return returns the type it points to as a string
 */
string Interpreter::extract_refType(string reference)
{
    string ptrType = reference.substr(reference.find('<') + 1, reference.find('>') - reference.find('<') - 1); //extract the type inside <>
    return ptrType;
}
