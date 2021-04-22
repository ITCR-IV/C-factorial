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

    //a pointer that iterates through the whole text eventually
    char *current_char;

    //the type of exception thrown by the lexer
    struct SyntaxException;

    void error(const string extraDetails);

    void skipWhitespace();

    string findInteger();

    Token findId();

    string findString();

    void advance();

public:
    //index of line
    int line;

    Lexer(string text);

    char *peek();

    Token getNextToken();
};

#endif // LEXER_H
