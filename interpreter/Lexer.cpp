#include "Lexer.h"

Lexer::Lexer(char *text)
{
    this->text = text;
    this->pos = 0;
    this->current_char = text[this->pos];
}

void error()
{
}