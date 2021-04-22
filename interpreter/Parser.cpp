#include "Parser.h"
#include "Lexer.h"
#include "Interpreter.h"

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