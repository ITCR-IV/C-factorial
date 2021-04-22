#include "Interpreter.h"
#include <string>

using namespace std;

/*!
 * \brief struct RuntimeException to define the exception thrown while interpreting
 * 
 */
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