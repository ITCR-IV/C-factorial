#ifndef LEXER_H
#define LEXER_H
#include <exception>
#include <string>
#include "Token.h"
#include "TokenConstants.h"

using namespace std;

//! Class that carries out the lexical analysis of the input code
class Lexer
{

private:
    //!Code string input
    string text;

    //!index of text
    int pos;

    //!a pointer that iterates through the whole text eventually
    char current_char;

    void error(const string extraDetails);

    void skipWhitespace();

    Token findNumber();

    Token findId();

    string findString();

    void advance();

public:
    //!struct SyntaxException to define the exception thrown by the lexer
    struct SyntaxException : public exception
    {

        SyntaxException(const string &msg) : msg_(msg) {}

        const char *what() const noexcept { return (&msg_[0]); }

    private:
        string msg_;
    };

    //!index of line
    int line;

    Lexer(string text);

    //char *peek();

    Token getNextToken();
};

#endif // LEXER_H
