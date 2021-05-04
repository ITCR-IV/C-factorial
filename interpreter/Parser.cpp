#include "Parser.h"
#include "Lexer.h"
#include "Interpreter.h"
#include <iostream>

//////////
/// For this parser I use this-> to reference attribute members but not for method calls
/// Also local variables have an _ succeeding them (for example "id_")
//////////

/*!
 * \brief Construct a new Parser object and initialize the first token and lexer
 * 
 * \param inputLexer must provide a fresh lexer to scan the code with
 */
Parser::Parser(Lexer inputLexer, MainWindow &window) : lexer(inputLexer), currentToken(this->lexer.getNextToken()), interpreter(window), insideStruct(false), scopeLevel(0) {}

/*!
 * \brief The lexer throws a SyntaxException if it can't tokenize properly
 * 
 * \param extraDetails Extra information relating to the error
 */
void Parser::error(const string extraDetails = "")
{
    string msg = "Semantic error found in line: ";
    if (this->currentToken.getValue() == EOL)
    {
        this->lexer.line--;
    }
    string lineNum = to_string(this->lexer.line);
    string fullMsg = msg + lineNum + '\n' + extraDetails + '\n';

    throw SemanticException(fullMsg);
}

/*!
 * \brief compare the current token type with the passed token type and if they match then "eat" the current token and assign the next token to the current_token, otherwise raise an exception.
 * 
 * \param tokenType expected token type
 */
void Parser::eat(string tokenType)
{
    if (this->currentToken.getType() == tokenType)
    {
        // cout << "\n\n\nExpected: \"" << tokenType << "\"" << endl
        //      << "Got: \"" << this->currentToken.getType() << "\"\n";
        this->currentToken = this->lexer.getNextToken();
    }
    else
    {
        string errormsg = "Expected a(n) '" + tokenType + "' and instead got '" + this->currentToken.getType() + "'";
        this->error(errormsg);
    }
}

/*!
 * \brief Evaluates an entire scope (don't use it if you plan on going line by line)
 * 
 */
void Parser::scope()
{
    printf("Entering scope \n");
    eat(LBRACK);
    if (this->currentToken.getType() == EOL)
    {
        eat(EOL);
    }

    this->interpreter.enter_scope();

    while (this->currentToken.getType() != RBRACK)
    {
        if (this->currentToken.getType() == LBRACK)
        {
            scope();
        }
        else
        {
            loc();
        }
    }

    eat(RBRACK);
    printf("Exiting scope\n");

    this->interpreter.exit_scope();
    if (this->currentToken.getType() == EOL)
    {
        eat(EOL);
    }

    return;
}

/*!
 * \brief Evaluates a line of code (again, don't use this for going line by line because it can't handle scopes itself and will skip over struct declarations)
 * 
 */
void Parser::loc()
{
    if (this->currentToken.getType() == SEMI)
    {
        eat(SEMI);
        eat(EOL);
        return;
    }

    if (this->currentToken.getType() == STRUCT)
    {
        struct_definition();
    }
    else if (this->currentToken.getType() == PRINT)
    {
        print_call();
    }
    else if (this->currentToken.getType() == STRUCTACCESS)
    {
        struct_declaration();
    }
    else
    {
        declaration();
    }

    eat(SEMI);
    eat(EOL);
    return;
}

/*!
 * \brief Handles the declaration of a new variable (not struct)
 * 
 */
void Parser::declaration()
{
    string type_ = type();

    if (this->currentToken.getType() == EQUAL) //this means that an existing variable is being changed, rather than a new variable being created
    {
        string id_ = type_;
        type_ = this->interpreter.getType(id_);
        eat(EQUAL);
        Token token_ = return_expression();
        string assignmentType_ = token_.getType();
        string assignmentValue_ = token_.getValue();
        this->interpreter.update_value(id_, assignmentType_, assignmentValue_);
        return;
    }

    if (this->currentToken.getType() != ID)
    {
        this->interpreter.getValue(type_); // this is to check that the lonely identifier is at least a declared variable
        cout << "Exiting because nothing's going on........";
        return;
    }

    string id_ = this->currentToken.getValue();
    eat(ID);

    string assignmentType_ = "";
    string assignmentValue_ = "";

    if (type_.find('<') != string::npos)
    { //So if it's a reference
        string ptrType_ = this->interpreter.extract_refType(type_);
        if (this->currentToken.getType() == EQUAL)
        {
            eat(EQUAL);

            Token token_ = return_expression();
            assignmentType_ = token_.getType();
            assignmentValue_ = token_.getValue();
            this->interpreter.reference_declaration(ptrType_, id_, assignmentType_, assignmentValue_);
            return;
        }
        this->interpreter.reference_declaration(ptrType_, id_, assignmentType_, assignmentValue_);
        return;
    }

    if (this->currentToken.getType() == EQUAL)
    {
        eat(EQUAL);

        Token token_ = return_expression();
        assignmentType_ = token_.getType();
        assignmentValue_ = token_.getValue();
        this->interpreter.declaration(type_, id_, assignmentType_, assignmentValue_);
        return;
    }

    this->interpreter.declaration(type_, id_, assignmentType_, assignmentValue_);
    return;
}

/*!
 * \brief evaluates an expression that returns a value, the types of the tokens returned by this method are the actual types that variables can be stored as (int, float, char, ...) instead of the Lexer token types
 * 
 * \return Token with type and value of the variable's or expression real types and values
 */
Token Parser::return_expression()
{
    if (this->currentToken.getType() == MINUS || this->currentToken.getType() == INTEGER || this->currentToken.getType() == DECIMAL)
    {
        return arithmetic();
    }
    else if (this->currentToken.getType() == ID)
    {
        string id_ = this->currentToken.getValue();
        if (this->interpreter.getType(id_) == INT || this->interpreter.getType(id_) == FLOAT || this->interpreter.getType(id_) == DOUBLE || this->interpreter.getType(id_) == LONG) //context-sensitive parsing
        {
            return arithmetic();
        }
        else
        {
            Token token_ = Token(this->interpreter.getType(id_), this->interpreter.getValue(id_));
            eat(ID);
            return token_;
        }
    }
    else if (this->currentToken.getType() == GETADDR || this->currentToken.getType() == GETVALUE)
    {
        return method_call();
    }
    else if (this->currentToken.getType() == STRING)
    {
        Token token_ = Token(CHAR, this->currentToken.getValue());
        eat(STRING);
        return token_;
    }
    else if (this->currentToken.getType() == STRUCTACCESS)
    {
        string id_ = this->currentToken.getValue();
        Token token_ = Token(this->interpreter.getType(id_), this->interpreter.getValue(id_));
        eat(STRUCTACCESS);
        return token_;
    }
    else
    {
        error("Unrecognized return expression");
    }
    return Token("Error in Parser.cpp::return_expression()", "Error in Parser.cpp::return_expression()");
}

/*!
 * \brief Evaluates the definition of a struct (don't use in line by line for same reason as scope())
 * 
 */
void Parser::struct_definition()
{
    eat(STRUCT);

    this->interpreter.enter_struct();

    struct_scope();

    string id_ = this->currentToken.getValue();
    eat(ID);
    this->interpreter.exit_struct(id_);
}

/*!
 * \brief Evaluates everything inside the scope of a struct definition
 * 
 */
void Parser::struct_scope()
{
    eat(LBRACK);

    if (this->currentToken.getType() == EOL)
    {
        eat(EOL);
    }
    while (this->currentToken.getType() != RBRACK)
    {
        struct_loc();
    }

    eat(RBRACK);
    return;
}

/*!
 * \brief Evaluate a line of code inside a struct, also can't be used for iterating 1 at a time, but it's because it doesn't support the other 1 at a time stuff
 * 
 */
void Parser::struct_loc()
{
    if (this->currentToken.getType() != SEMI)
    {
        declaration();
    }

    eat(SEMI);
    eat(EOL);
    return;
}

/*!
 * \brief For when an attribute of a struct needs to be changed
 * 
 */
void Parser::struct_declaration()
{
    string structAccess_ = this->currentToken.getValue();

    eat(STRUCTACCESS);

    eat(EQUAL);
    Token token_ = return_expression();
    string assignmentType_ = token_.getType();
    string assignmentValue_ = token_.getValue();

    this->interpreter.update_value(structAccess_, assignmentType_, assignmentValue_);
    return;
}

/*!S
 * \brief Evaluates an arithmetic expression calling the interpreter arithmetic() method in case there's an operator
 * 
 * \return Token with type and value of the result of the expression;
 */
Token Parser::arithmetic()
{
    Token result = factor();

    while (this->currentToken.getType() == MUL || this->currentToken.getType() == DIV || this->currentToken.getType() == PLUS || this->currentToken.getType() == MINUS)
    {
        if (this->currentToken.getType() == MUL)
        {
            eat(MUL);
            Token op2 = factor();
            result = this->interpreter.arithmetic('*', result, op2);
        }
        else if (this->currentToken.getType() == DIV)
        {
            eat(DIV);
            Token op2 = factor();
            result = this->interpreter.arithmetic('/', result, op2);
        }
        else if (this->currentToken.getType() == PLUS)
        {
            eat(PLUS);
            Token op2 = factor();
            result = this->interpreter.arithmetic('+', result, op2);
        }
        else if (this->currentToken.getType() == MINUS)
        {
            eat(MINUS);
            Token op2 = factor();
            result = this->interpreter.arithmetic('-', result, op2);
        }
    }
    return result;
}

/*!
 * \brief Deals with the getAddr and getValue functions
 * 
 * \return Token of type and value, if getAddr gets called the type is a reference (format: "reference<type>") and value is the address
 */
Token Parser::method_call()
{
    if (this->currentToken.getType() == GETVALUE)
    {
        eat(GETVALUE);
        eat(LPAREN);
        if (this->currentToken.getType() == STRUCTACCESS || this->currentToken.getType() == ID)
        {
            Token token_ = this->currentToken;
            if (this->interpreter.getType(token_.getValue()).find('<') == string::npos)
            {
                error("getValue() method called for non-reference type");
            }
            eat(this->currentToken.getType());
            eat(RPAREN);
            return Token(this->interpreter.getType(token_.getValue()), this->interpreter.getRefValue(token_.getValue()));
        }
        else
        {
            error("Wrong parameter to getValue method");
        }
    }
    else if (this->currentToken.getType() == GETADDR)
    {
        eat(GETADDR);
        eat(LPAREN);
        if (this->currentToken.getType() == STRUCTACCESS || this->currentToken.getType() == ID)
        {
            Token token_ = this->currentToken;
            if (this->interpreter.getType(token_.getValue()).find('<') != string::npos)
            {
                error("getAddr() method called for a reference type");
            }
            eat(this->currentToken.getType());
            eat(RPAREN);
            return Token(REFERENCE + "<" + this->interpreter.getType(token_.getValue()) + ">", this->interpreter.getAddr(token_.getValue()));
        }
        else
        {
            error("Wrong parameter to getAddr method");
        }
    }
    else
    {
        printf("Something wrong in Parser::method_call()\n");
        error("Something wrong in Parser::method_call()\n");
    }
    return Token("Error in Parser.cpp::method_call()", "Error in Parser.cpp::method_call()");
}

/*!
 * \brief Deals with print() function, calls the interpreter print_call() method
 * 
 */
void Parser::print_call()
{
    eat(PRINT);
    eat(LPAREN);
    string printString_ = return_expression().getValue();
    eat(RPAREN);

    this->interpreter.print_call(printString_);
}

/*!
 * \brief Evaluates tokens to return a straight number or a number stored in a variable
 * 
 * \return Token where type is the type of number (int,long,double,float) and value is... the value...
 */
Token Parser::factor()
{
    if (this->currentToken.getType() == MINUS || this->currentToken.getType() == INTEGER || this->currentToken.getType() == DECIMAL)
    {
        return number();
    }
    else if (this->currentToken.getType() == ID)
    {
        string id_ = this->currentToken.getValue();
        eat(ID);
        return Token(this->interpreter.getType(id_), this->interpreter.getValue(id_));
    }
    else
    {
        printf("Something wrong in Parser::factor()\n");
        error("Something wrong in Parser::factor()\n");
    }
    return Token("Error in Parser.cpp::factor()", "Error in Parser.cpp::factor()");
}

/*!
 * \brief Evaluates expressions that are numbers and returns a token representing the number
 * 
 * \return Token where type is the type of number (int,long,double,float) and value is... the value...
 */
Token Parser::number()
{
    string result = "";

    if (this->currentToken.getType() == MINUS)
    {
        result += this->currentToken.getValue();
        eat(MINUS);
    }
    if (this->currentToken.getType() == INTEGER)
    {
        result += this->currentToken.getValue();
        eat(INTEGER);
        return Token(INT, result);
    }
    else if (this->currentToken.getType() == DECIMAL)
    {
        result += this->currentToken.getValue();
        eat(DECIMAL);
        return Token(FLOAT, result);
    }
    else
    {
        printf("Something wrong in Parser::number()\n");
        error("Something wrong in Parser::number()\n");
    }
    return Token("Error in Parser.cpp::number()", "Error in Parser.cpp::number()");
}

/*!
 * \brief It evaluates a type id that may be a keyword or a struct id, also references get evaluated as a special case due to being succeeded by <type>
 * 
 * \return string of the type
 */
string Parser::type()
{
    string type_ = this->currentToken.getType();
    if (type_ == INT || type_ == LONG || type_ == CHAR || type_ == FLOAT || type_ == FLOAT || type_ == DOUBLE)
    {
        eat(type_);
        return type_;
    }
    else if (this->currentToken.getType() == REFERENCE)
    {
        eat(REFERENCE);
        eat(LARROW);
        string refType_ = type();
        eat(RARROW);
        return type_ + "<" + refType_ + ">";
    }
    else if (this->currentToken.getType() == ID)
    {
        type_ = this->currentToken.getValue();
        eat(ID);
        return type_;
    }
    else
    {
        printf("Something wrong in Parser::type()\n");
        //cout << "TYPE: " << this->currentToken.getType();
        error("Something wrong in Parser::type()\n");
    }
    return "Error in Parser.cpp::type()";
}

///////////////////////
/// Methods for going line by line
///////////////////////

/*!
 * \brief Method for handling a loc that isn't inside the struct while going line by line
 * 
 */
void Parser::loc1()
{
    if (this->currentToken.getType() == STRUCT)
    {
        struct_definition_open1();
    }
    else if (this->currentToken.getType() == LBRACK)
    {
        eat(LBRACK);
        this->interpreter.enter_scope();
        this->scopeLevel++;

        eat(EOL);
    }
    else if (this->currentToken.getType() == RBRACK)
    {
        eat(RBRACK);
        this->interpreter.exit_scope();
        this->scopeLevel--;

        eat(EOL);
    }
    else if (this->currentToken.getType() == EOFF)
    {
        if (this->insideStruct)
        {
            error("Reached EOF while defining struct");
        }
        if (this->scopeLevel > 0)
        {
            error("Reached EOF with unclosed scope");
        }
        else if (this->scopeLevel < 0)
        {
            printf("Reached EOF with negative scope ._.");
        }
        eat(EOFF);
    }
    else
    {
        if (this->currentToken.getType() == SEMI)
        {
            eat(SEMI);
            eat(EOL);
            return;
        }
        if (this->currentToken.getType() == PRINT)
        {
            print_call();
        }
        else if (this->currentToken.getType() == STRUCTACCESS)
        {
            struct_declaration();
        }
        else
        {
            declaration();
        }

        eat(SEMI);
        eat(EOL);
        return;
    }
    return;
}

/*!
 * \brief Method for handling a loc while inside a struct while going line by line, the difference is that it can accept a struct_definition_close1 rule
 * 
 */
void Parser::struct_loc1()
{
    if (this->currentToken.getType() != SEMI)
    {
        if (this->currentToken.getType() == RBRACK)
        {
            struct_definition_close1();
        }
        else
        {
            declaration();
        }
    }

    eat(SEMI);
    eat(EOL);
    return;
}

/*!
 * \brief Method for starting a struct definition when going line by line
 * 
 */
void Parser::struct_definition_open1()
{
    eat(STRUCT);

    eat(LBRACK);
    eat(EOL);

    this->insideStruct = true;
    this->interpreter.enter_struct();
}

/*!
 * \brief Method for closing a struct definition when going line by line
 * 
 */
void Parser::struct_definition_close1()
{
    eat(RBRACK);

    string id_ = this->currentToken.getValue();
    eat(ID);
    this->insideStruct = false;
    this->interpreter.exit_struct(id_);
}

/*!
 * \brief Call this method to advance just one line of code when in line by line mode
 * 
 */
void Parser::advance_1loc()
{
    if (this->insideStruct)
    {
        struct_loc1();
    }
    else
    {
        loc1();
    }
}