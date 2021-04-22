#include "Parser.h"
#include "Lexer.h"

/*!
 * \brief Construct a new Parser object and initialize the first token and lexer
 * 
 * \param inputLexer must provide a fresh lexer to scan the code with
 */
Parser::Parser(Lexer inputLexer) : lexer(inputLexer), currentToken(this->lexer.getNextToken()) {}