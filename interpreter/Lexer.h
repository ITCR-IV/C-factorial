#ifndef LEXER_H
#define LEXER_H
#include <exception>
#include <string>
#include "Token.h"
#include "TokenConstants.h"

using namespace std;

/*!
 * \brief Class that carries out the lexical analysis of the input code
 * 
 */
class Lexer
{

private:
    //Code string input
    string text;

    //index of text
    int pos;

    //index of line
    int line;

    char *current_char;

    struct SyntaxException;

    void error(const string extraDetails);

public:
    Lexer(string text);

    void advance();

    char *peek();

    void skipWhitespace();

    string findInteger();

    Token findId();

    string findString();

    Token getNextToken();
};

#endif // LEXER_H
