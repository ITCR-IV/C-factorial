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

    this->interpreter.exit_scope();
}

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

    /*
        else if (this->currentToken.getValue() == "getAddr" || this->currentToken.getValue() == "getValue")
        {
            return_expression();
        }
        else if (this->currentToken.getType() == MINUS)
        {
            return_expression();
        }*/
}

void Parser::declaration()
{
    string type_ = type().getValue();

    string id_ = this->currentToken.getValue();
    eat(ID);

    string value_ = nullptr;

    if (this->currentToken.getType() == EQUAL)
    {
        eat(EQUAL);
        value_ = return_expression().getValue();
    }

    this->interpreter.declaration(type_, id_, value_);
    return;
}

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

void Parser::struct_definition()
{
    eat(STRUCT);

    this->interpreter.enter_struct();

    struct_scope();

    string id_ = this->currentToken.getValue();
    eat(ID);
    this->interpreter.exit_struct(id_);
}

void Parser::struct_loc() {}

void Parser::struct_scope() {}

void Parser::struct_declaration() {}

Token Parser::struct_access() {}

Token Parser::arithmetic() {}

Token Parser::method_call() {}

void Parser::print_call() {}

Token Parser::factor() {}

Token Parser::number() {}

Token Parser::type() {}