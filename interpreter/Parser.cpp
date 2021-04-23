#include "Parser.h"
#include "Lexer.h"
#include "Interpreter.h"

//////////
/// For this parser I use this-> to reference attribute members but not for method calls
/// Also local variables have an _ succeeding them (for example "id_")
//////////

/*!
 * \brief Construct a new Parser object and initialize the first token and lexer
 * 
 * \param inputLexer must provide a fresh lexer to scan the code with
 */
Parser::Parser(Lexer inputLexer) : lexer(inputLexer), currentToken(this->lexer.getNextToken()), interpreter(Interpreter()) {}

/*!
 * \brief struct SemanticException to define the exception thrown by the parser
 * 
 */
struct Parser::SemanticException : public exception
{

    SemanticException(const string &msg) : msg_(msg) {}

    string getMessage() const { return (msg_); }

private:
    string msg_;
};

/*!
 * \brief The lexer throws a SyntaxException if it can't tokenize properly
 * 
 * \param extraDetails Extra information relating to the error
 */
void Parser::error(const string extraDetails = "")
{
    string msg = "Semantic error found in line: ";
    string lineNum = to_string(this->lexer.line);
    string fullMsg = msg + lineNum + '\n' + extraDetails;

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
        this->currentToken = this->lexer.getNextToken();
    }
    else
    {
        string errormsg = "Expected a(n) '" + tokenType + "' and instead got '" + this->currentToken.getType() + "'";
        this->error();
    }
}

/*!
 * \brief Evaluates an entire scope (don't use it if you plan on going line by line)
 * 
 */
void Parser::scope()
{
    eat(LBRACK);
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

    string id_ = this->currentToken.getValue();
    eat(ID);

    string assignmentType_ = nullptr;
    string assignmentValue_ = nullptr;

    if (this->currentToken.getType() == EQUAL)
    {
        eat(EQUAL);
        Token token_ = return_expression();
        string assignmentType_ = token_.getType();
        string assignmentValue_ = token_.getValue();
    }

    this->interpreter.declaration(type_, id_, assignmentType_, assignmentValue_);
    return;
}

/*!
 * \brief evaluates an expression that returns a value
 * 
 * \return Token with type and value of return
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
            Token token_ = this->currentToken;
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
        Token token_ = this->currentToken;
        eat(STRING);
        return token_;
    }
    else if (this->currentToken.getType() == STRUCTACCESS)
    {
        Token token_ = this->currentToken;
        eat(STRUCTACCESS);
        return token_;
    }
    else
    {
        error("Unrecognized expression");
    }
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
 * \brief Evaluate a line of code inside a struct, unlike regular loc this _can_ be used to go line by line (because it doesn't accept structs or anything scoped)
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

    this->interpreter.struct_declaration(structAccess_, assignmentType_, assignmentValue_);
    return;
}

/*!
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
 * \return Token of type and value, if getAddr gets called the type is a reference and value is the address
 */
Token Parser::method_call()
{
    if (this->currentToken.getType() == GETVALUE)
    {
        eat(GETVALUE);
        eat(LPAREN);
        if (this->currentToken.getType() == STRUCTACCESS || this->currentToken.getType() == ID)
        {
            return Token(this->interpreter.getType(this->currentToken.getValue()), this->interpreter.getRefValue(this->currentToken.getValue()));
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
            return Token(REFERENCE + "<" + this->interpreter.getType(this->currentToken.getValue()) + ">", this->interpreter.getAddr(this->currentToken.getValue()));
        }
        else
        {
            error("Wrong parameter to getAddr method");
        }
    }
    else
    {
        printf("Something wrong in Parser::method_call()\n");
        error();
    }
}

void Parser::print_call()
{
    eat(PRINT);
    eat(LPAREN);
    string printString_ = return_expression().getValue();
    eat(RPAREN);

    this->interpreter.print_call(printString_);
}

Token Parser::factor()
{
    if (this->currentToken.getType() == MINUS || this->currentToken.getType() == INTEGER || this->currentToken.getType() == DECIMAL)
    {
        return number();
    }
    else if (this->currentToken.getType() == ID)
    {
        string id_ = this->currentToken.getValue();
        return Token(this->interpreter.getType(id_), this->interpreter.getValue(id_));
    }
    else
    {
        printf("Something wrong in Parser::factor()\n");
        error();
    }
}

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
        return Token(INT, result);
    }
    else if (this->currentToken.getType() == DECIMAL)
    {
        result += this->currentToken.getValue();
        return Token(FLOAT, result);
    }
    else
    {
        printf("Something wrong in Parser::number()\n");
        error();
    }
}

// type = "int" | "long" | "char" | "float" | "double" | ("reference" , "<" , type , ">") | ID
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
        return this->currentToken.getValue();
    }
    else
    {
        printf("Something wrong in Parser::type()\n");
        error();
    }
}