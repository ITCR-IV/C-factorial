#include "Lexer.h"
#include <string>

using namespace std;

Lexer::Lexer(char *text)
{
    this->text = text;
    this->pos = 0;
    this->current_char = text[this->pos];
    this->line = 1;
}

void Lexer::error()
{
    string msg = "Syntax error found in line: ";
    string lineNum = to_string(this->line);

    string fullMsg = msg + lineNum;

    throw SyntaxException(fullMsg);
}