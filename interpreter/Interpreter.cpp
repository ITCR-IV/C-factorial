#include "Interpreter.h"
#include <string>
#include "ServerManager.h"
#include "Token.h"
#include "TokenConstants.h"
#include "server/JsonDecoder.h"
#include "server/JsonEncoder.h"
#include "server/RequestConstants.h"
#include "Mainwindow.h"
#include "Logger.h"

using namespace std;

/*!
 * \brief constructor of the Interpreter class
 *
 */
Interpreter::Interpreter(MainWindow &guiWindow) : manager(ServerManager::getInstance()), guiWindow(&guiWindow) {}

/*!
 * \brief The interpreter throws a RuntimeException if an expression is invalid
 * 
 * \param extraDetails Extra information relating to the error
 */
void Interpreter::error(const string extraDetails = "")
{
    if (!guiWindow->isRunning)
    {
        return; // This is so it the interpreter won't raise runtime exceptions when just doing a syntax/semantic check
    }
    string msg = "Runtime error";
    string fullMsg = msg + '\n' + extraDetails + '\n';

    throw RuntimeException(fullMsg);
}

/*!
 * \brief Tell the memory server to increment the scope stack so that it can assign new variables in the new scope
 * 
 */
void Interpreter::enter_scope()
{
    manager->sendRequest(ENTERSCOPE);
    return;
}

/*!
 * \brief Tell the memory server to descrease scope stack and release all memory pertaining to the exited scope
 * 
 */
void Interpreter::exit_scope()
{
    manager->sendRequest(EXITSCOPE);
    return;
}

/*!
* \brief  Tell the memory server to allocate a variable defined by the parameters of this method, if the id is already defined then throw an exception
* 
* \param type the type that is being declared
* \param id the id of the new variable
* \param assignType the type of the value being assigned
* \param value the value being assigned
*/
void Interpreter::declaration(string type, string id, string assignType /*= ""*/, string value /*= ""*/)
{
    if (assignType != "")
    {
        if (type != assignType)
        {
            if (type == LONG && assignType == INT)
            {
                ;
            }
            else if (type == DOUBLE && (assignType == FLOAT || assignType == INT))
            {
                ;
            }
            else if (type == FLOAT && assignType == INT)
            {
                ;
            }
            else
            {
                string msg = "Wrong type assignment, attempting to assign '" + assignType + "' to '" + type + "'";
                error(msg);
            }
        }
        UpdateInfo varDeclare = UpdateInfo(type, id, value);

        JsonEncoder encoder = JsonEncoder(varDeclare);
        manager->sendRequest(DECLARE);
        manager->sendJson(encoder.encode());
        if (stoi(manager->getServerMsg()) == ERROR)
        {
            error("Couldn't declare the given variable, already stored in memory");
        }
        return;
    }
    else
    {
        //Default values are always ""
        std::string defValue = "";

        UpdateInfo varDeclare = UpdateInfo(type, id, defValue);
        JsonEncoder encoder = JsonEncoder(varDeclare);
        manager->sendRequest(DECLARE);
        manager->sendJson(encoder.encode());
        if (stoi(manager->getServerMsg()) == ERROR)
        {
            error("Couldn't declare the given variable, already stored in memory");
        }
        return;
    }
}

/*!
* \brief Tell the memory server to allocate a variable defined by the parameters of this method, or if the id is already defined then change it's value. This method works for regular variables, structaccesses and reference variables
* 
* \param ptrType the type that the reference points to, this is just to check that the type it points to matches the type of reference assigned (the type inside <>, without the <>); Example: int
* \param id the id of the new reference variable
* \param assignType the type of the value being assigned, this is to check that it IS a reference and that the type inside <> matches the ptrType; Example: reference<int>
* \param value the value being assigned
*/
void Interpreter::reference_declaration(string ptrType, string id, string assignType /*= ""*/, string value /*= ""*/) //default value for references is 0
{
    if (assignType != "")
    {
        if (assignType.find('<') == string::npos)
        {
            error("Trying to assing non-reference to reference type");
        }
        string assignPtrType = this->extract_refType(assignType);

        if (ptrType != assignPtrType)
        {
            string msg = "Wrong type assignment, attempting to assign '" + assignPtrType + "' reference to '" + ptrType + "' reference";
            error(msg);
        }
        UpdateInfo varDeclare = UpdateInfo(ptrType, id, value);

        JsonEncoder encoder = JsonEncoder(varDeclare);
        manager->sendRequest(DECLAREREF);
        manager->sendJson(encoder.encode());
        if (stoi(manager->getServerMsg()) == ERROR)
        {
            error("Couldn't declare the given variable, already stored in memory or type undefined");
        }
        return;
    }
    else
    {
        //Default values are always ""
        std::string defValue = "";

        UpdateInfo varDeclare = UpdateInfo(ptrType, id, defValue);
        JsonEncoder encoder = JsonEncoder(varDeclare);
        manager->sendRequest(DECLARE);
        manager->sendJson(encoder.encode());
        if (stoi(manager->getServerMsg()) == ERROR)
        {
            error("Couldn't declare the given variable, already stored in memory or type undefined");
        }
        return;
    }
}

/*!
* \brief  Tell the memory server to change the value of an existing variable, if the id isn't already defined then throw an exception
* 
* \param id the id of the variable
* \param assignType the type of the value being assigned
* \param value the value being assigned
 */
void Interpreter::update_value(string id, string assignType, string value)
{
    string type = getType(id);
    if (type != assignType)
    {
        if (type == LONG && assignType == INT)
        {
            ;
        }
        else if (type == DOUBLE && (assignType == FLOAT || assignType == INT))
        {
            ;
        }
        else if (type == FLOAT && assignType == INT)
        {
            ;
        }
        else
        {
            string msg = "Wrong type assignment, attempting to assign '" + assignType + "' to '" + type + "'";
            error(msg);
        }
    }
    UpdateInfo varChange = UpdateInfo(type, id, value);

    JsonEncoder encoder = JsonEncoder(varChange);
    manager->sendRequest(CHANGEVALUE);
    manager->sendJson(encoder.encode());
    if (stoi(manager->getServerMsg()) == ERROR)
    {
        error("Can't change variable that wasn't declared previously");
    }
    return;
}

/*!
 * \brief Tell the memory server that the following declarations are for attributes of a struct being constructed
 * 
 */
void Interpreter::enter_struct()
{
    manager->sendRequest(ENTERSTRUCT);
    return;
}

/*!
 * \brief Tell the memory server to stop assigning the declarations to the struct type and finishes defining the struct by giving it an id
 * 
 * \param id The identifier of the struct type
 */
void Interpreter::exit_struct(string id)
{
    manager->sendRequest(EXITSTRUCT);
    manager->sendJson(id);
    if (stoi(manager->getServerMsg()) == ERROR)
    {
        string msg = "Previous definition exists for struct type '" + id + "'";
        error(msg);
    }
    return;
}

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
string Interpreter::getAddr(string id)
{
    manager->sendRequest(REQUESTVARIABLE);
    manager->sendJson(id);
    std::string info = manager->getServerMsg();
    if (info == "-1")
    {
        error("Requesting undeclared variable");
        return "0";
    }
    JsonDecoder decoder = JsonDecoder(info);
    UpdateInfo idInfo = decoder.decode();
    return to_string(idInfo.getDataAddress());
}

/*!
 * \brief Returns the value of a given variable, if no such variable has been defined throws error; for structs (not their attributes but the actual struct) this function will return their address
 * 
 * \param id is a variable and can be a plain variable or a struct access refering to a variable inside a struct
 * \return string identifier of the variable
 */
string Interpreter::getValue(string id)
{
    manager->sendRequest(REQUESTVARIABLE);
    manager->sendJson(id);
    std::string info = manager->getServerMsg();
    if (info == "-1")
    {
        error("Requesting undeclared variable");
        Logger::EnableFileOutput();
        Logger::Error("Llamado a una variable no declarada");
        return "0";
    }
    JsonDecoder decoder = JsonDecoder(info);
    UpdateInfo idInfo = decoder.decode();
    return idInfo.getDataValue();
}

/*!
 * \brief dereference the reference type variables
 * 
 * \param id the id of the reference variable
 * \return string containing the value of the variable the reference points to
 */
string Interpreter::getRefValue(string id)
{
    // Check that a reference type is passed
    // Still need to add to server a method to send back the info for a variable by giving it's address
    return id;
}

/*!
 * \brief Asks the memory server for the type of a variable
 * 
 * \param id variable id
 * \return string with type, must correspond to one of the types defined in TokenConstants.h
 */
string Interpreter::getType(string id)
{
    manager->sendRequest(REQUESTVARIABLE);
    manager->sendJson(id);
    std::string info = manager->getServerMsg();
    if (info == "-1")
    {
        error("Requesting variable not stored in memory");
        return "0";
    }
    JsonDecoder decoder = JsonDecoder(info);
    UpdateInfo idInfo = decoder.decode();
    return idInfo.getDataType();
}

/*!
 * \brief Prints to Stdout whatever is passed to it
 * 
 * \param msg msg that is to be printed
 */
void Interpreter::print_call(string msg)
{
    guiWindow->append_stdout_line(msg);
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
