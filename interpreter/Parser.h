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

    //the type of exception thrown by the parser
    struct SemanticException;

    void error(const string extraDetails);

    void eat(string tokenType);

public:
    Parser(Lexer inputLexer);

    void scope();

    void loc();

    void declaration();

    Token return_expression();

    void struct_definition();

    void struct_loc();

    void struct_scope();

    void struct_declaration();

    Token struct_access();

    Token arithmetic();

    Token method_call();

    void print_call();

    Token factor();

    Token number();

    Token type();
};

#endif