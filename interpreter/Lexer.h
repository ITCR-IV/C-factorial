#ifndef LEXER_H
#define LEXER_H
#include <exception>
#include <string>
#include "Token.h"
#include "TokenConstants.h"

using namespace std;

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

    struct SyntaxException : public exception
    {
        SyntaxException(const string &msg) : msg_(msg) {}

        string getMessage() const { return (msg_); }

    private:
        string msg_;
    };

    void error();

public:
    Lexer(string text);

    void advance();

    char *peek();

    void skipWhitespace();

    int findInteger();

    Token findId();

    string findString();

    Token getNextToken();

    /*

    def skip_whitespace(self):
        while self.current_char is not None and self.current_char.isspace():
            self.advance()

    def integer(self):
        """Return a (multidigit) integer consumed from the input."""
        result = ''
        while self.current_char is not None and self.current_char.isdigit():
            result += self.current_char
            self.advance()
        return int(result)

    def get_next_token(self):
        """Lexical analyzer (also known as scanner or tokenizer)

        This method is responsible for breaking a sentence
        apart into tokens. One token at a time.
        """
        while self.current_char is not None:

            if self.current_char.isspace():
                self.skip_whitespace()
                continue

            if self.current_char.isdigit():
                return Token(INTEGER, self.integer())

            if self.current_char == '+':
                self.advance()
                return Token(PLUS, '+')

            if self.current_char == '-':
                self.advance()
                return Token(MINUS, '-')

            if self.current_char == '*':
                self.advance()
                return Token(MUL, '*')

            if self.current_char == '/':
                self.advance()
                return Token(DIV, '/')

            self.error()

        return Token(EOF, None)
        */
};

#endif // LEXER_H
