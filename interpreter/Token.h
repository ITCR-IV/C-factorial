#ifndef TOKEN_H
#define TOKEN_H

class Token
{
private:
    char *type;

    char *value;

public:
    Token(char *type, char *value);

    char *getType();

    char *getValue();
};

#endif