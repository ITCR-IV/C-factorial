#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Token.h"
#include "Interpreter.h"

class Parser
{

private:
    //! Lexer employed by parser
    Lexer lexer;

    //! token the parser is going over
    Token currentToken;

    //! Interpreter employed by parser
    Interpreter interpreter;

    //! Flag so when going line by line parser knows which kind of loc to except (can vary between loc1 and struct_loc)
    bool insideStruct;

    //! Value so when going line by line the parser knows how deep it is and when it gets to end if all scopes were closed
    int scopeLevel;

    void error(const string extraDetails);

    void eat(string tokenType);

public:
    Parser(Lexer inputLexer);

    //the type of exception thrown by the parser
    struct SemanticException;

    void scope();

    void loc();

    void declaration();

    Token return_expression();

    void struct_definition();

    void struct_scope();

    void struct_loc();

    void struct_declaration();

    Token arithmetic();

    Token method_call();

    void print_call();

    Token factor();

    Token number();

    string type();

    //Methods for iterating 1 line at a time:

    void loc1();

    void struct_loc1();

    void struct_definition_open1();

    void struct_definition_close1();

    void advance_1loc();
};

#endif