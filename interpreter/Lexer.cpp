#include "Lexer.h"
#include <cstring>
#include <string>

using namespace std;

Lexer::Lexer(string text)
{
    this->text = text;
    this->pos = 0;
    this->current_char = &text[this->pos];
    this->line = 1;
}

void Lexer::error()
{
    string msg = "Syntax error found in line: ";
    string lineNum = to_string(this->line);

    string fullMsg = msg + lineNum;

    throw SyntaxException(fullMsg);
}

void Lexer::advance()
{
    this->pos++;
    if (this->pos > this->text.length())
    {
        this->current_char = nullptr;
    }
    else
    {
        this->current_char++;
    }
}

void Lexer::skipWhitespace()
{
    while (current_char != nullptr && *current_char == ' ')
    {
        this->advance();
    }
}