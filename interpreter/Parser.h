#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Token.h"
#include "Interpreter.h"

//! Class that is in charge of parsing and interpreting the C! language, even though there's a Parser class and an Interpreter class these divisions are only for modularization convenience, in reality because the Parser directly owns and calls the interpreter methods it classifies as a syntax-directed interpreter
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

    void error(const string extraDetails);

    void eat(string tokenType);

public:
    Parser(Lexer inputLexer, MainWindow &window);

    //! Value so when going line by line the parser knows how deep it is and when it gets to end if all scopes were closed
    int scopeLevel;

    //!struct SemanticException to define the exception thrown by the parser
    struct SemanticException : public exception
    {

        SemanticException(const string &msg) : msg_(msg) {}

        const char *what() const noexcept { return (&msg_[0]); }

    private:
        string msg_;
    };

private:
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

public:
    void advance_1loc();
    void scope();
};

#endif