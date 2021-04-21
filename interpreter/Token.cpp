#include "Token.h"

Token::Token(char *type, char *value)
{
    this->type = type;
    this->value = value;
}

char *Token::getType()
{
    return this->type;
}

char *Token::getValue()
{
    return this->value;
}