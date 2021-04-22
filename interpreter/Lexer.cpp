#include "Lexer.h"
#include <cstring>
#include <string>
#include <ctype.h>

using namespace std;

/*!
 * \brief Construct a new Lexer object
 * 
 * \param text The entire code text
 */
Lexer::Lexer(string text)
{
    this->text = text;
    this->pos = 0;
    this->current_char = &text[this->pos];
    this->line = 1;
}

/*!
 * \brief struct SyntaxException to define the exception thrown by the lexer
 * 
 */
struct Lexer::SyntaxException : public exception
{

    SyntaxException(const string &msg) : msg_(msg) {}

    string getMessage() const { return (msg_); }

private:
    string msg_;
};

/*!
 * \brief The lexer throws a SyntaxException if it can't tokenize properly
 * 
 */
void Lexer::error()
{
    string msg = "Syntax error found in line: ";
    string lineNum = to_string(this->line);

    string fullMsg = msg + lineNum;

    throw SyntaxException(fullMsg);
}

/*!
 * \brief moves the current_char pointer and adds 1 to pos counter
 * 
 */
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

/*!
 * \brief check the following char in the text string
 * 
 * \return a char* to the character next to the current_char, if the file is over then it returns nullptr 
 */
char *Lexer::peek()
{
    int peekPos = this->pos + 1;
    if (peekPos > this->text.length())
    {
        return nullptr;
    }
    else
    {
        return this->current_char + 1;
    }
}

/**
 * \brief skips all spaces that appear together when called
 * 
 */
void Lexer::skipWhitespace()
{
    while (this->current_char != nullptr && *(this->current_char) == ' ')
    {
        this->advance();
    }
}

/*!
 * \brief scans the text for an integer
 * 
 * \return string representing integer that was found in the text
 */
string Lexer::findInteger()
{
    string result = "";
    while (this->current_char != nullptr && isdigit(*(this->current_char)))
    {
        result += *(this->current_char);
        this->advance();
    }
    return result;
}

/*!
 * \brief scans the text for a complete identifier
 * 
 * \return Token token type is ID, unless the ID was a reserved word, in which case the type is the reserved word
 */
Token Lexer::findId()
{
    string result = "";
    while (this->current_char != nullptr && isalnum(*(this->current_char)))
    {
        result += *(this->current_char);
        this->advance();
    }
    if (RESERVED_KEYWORDS.find(result) == RESERVED_KEYWORDS.end())
    {
        return Token(ID, result);
    }
    else
    {
        return Token(result, result);
    }
}

/*!
 * \brief scans the text for a full opened and closed string
 * 
 * \return string string that was found, if string isn't closed it'll throw an error
 */
string Lexer::findString()
{
    string result = "\"";
    this->advance();
    while (*(this->current_char) != '\"')
    {
        if (*(this->current_char) == '\n' || this->current_char != nullptr) //Reached end of line or EOF without closing string
        {
            this->error();
        }
        result += *(this->current_char);
        this->advance();
    }
    result += *(this->current_char);
    this->advance();

    return result;
}

/*!
 * \brief This is the lexical analyzer (tokenizer) method
 * 
 * \return Token The next token in the input code
 */
Token Lexer::getNextToken()
{
    while (this->current_char != nullptr)
    {
        if (*(this->current_char) == ' ')
        {
            this->skipWhitespace();
            continue;
        }
        if (*(this->current_char) == '\n')
        {
            this->line++;
            this->advance();
            continue;
        }
        if (isalpha(*(this->current_char)))
        {
            return this->findId();
        }
        if (isdigit(*(this->current_char)))
        {
            return Token(INTEGER, this->findInteger());
        }
        if (*(this->current_char) == '"')
        {
            return Token(STRING, this->findString());
        }
        if (*(this->current_char) == '{')
        {
            this->advance();
            return Token(LBRACK, LBRACK);
        }
        if (*(this->current_char) == '}')
        {
            this->advance();
            return Token(RBRACK, RBRACK);
        }
        if (*(this->current_char) == '(')
        {
            this->advance();
            return Token(LPAREN, LPAREN);
        }
        if (*(this->current_char) == ')')
        {
            this->advance();
            return Token(RPAREN, RPAREN);
        }
        if (*(this->current_char) == '<')
        {
            this->advance();
            return Token(LARROW, LARROW);
        }
        if (*(this->current_char) == '>')
        {
            this->advance();
            return Token(RARROW, RARROW);
        }
        if (*(this->current_char) == '=')
        {
            this->advance();
            return Token(EQUAL, EQUAL);
        }
        if (*(this->current_char) == '.')
        {
            this->advance();
            return Token(DOT, DOT);
        }
        if (*(this->current_char) == ';')
        {
            this->advance();
            return Token(SEMI, SEMI);
        }
        if (*(this->current_char) == '+')
        {
            this->advance();
            return Token(PLUS, PLUS);
        }
        if (*(this->current_char) == '-')
        {
            this->advance();
            return Token(MINUS, MINUS);
        }
        if (*(this->current_char) == '*')
        {
            this->advance();
            return Token(MUL, MUL);
        }
        if (*(this->current_char) == '/')
        {
            this->advance();
            return Token(DIV, DIV);
        }
        this->error();
    }
    return Token(EOFF, nullptr);
}