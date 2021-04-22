#include "Parser.h"
#include "Lexer.h"

/*!
 * \brief Construct a new Parser object and initialize the first token and lexer
 * 
 * \param inputLexer must provide a fresh lexer to scan the code with
 */
Parser::Parser(Lexer inputLexer, int PORT) : lexer(inputLexer), currentToken(this->lexer.getNextToken()), interpreter(Interpreter(PORT)) {}

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