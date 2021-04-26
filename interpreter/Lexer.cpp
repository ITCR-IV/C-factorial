#include "Lexer.h"
#include <cstring>
#include <string>
#include <ctype.h>

#include <iostream>

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
    //cout << "Length: " << this->text.length() << endl;
    this->line = 1;
}

/*!
 * \brief struct SyntaxException to define the exception thrown by the lexer
 * 
 */
struct Lexer::SyntaxException : public exception
{

    SyntaxException(const string &msg) : msg_(msg) {}

    const char *what() const noexcept { return (&msg_[0]); }

private:
    string msg_;
};

/*!
 * \brief The lexer throws a SyntaxException if it can't tokenize properly
 * 
 * \param extraDetails Extra information relating to the error
 */
void Lexer::error(const string extraDetails = "")
{
    string msg = "Syntax error found in line: ";
    string lineNum = to_string(this->line);

    string fullMsg = msg + lineNum + '\n' + extraDetails;

    throw SyntaxException(fullMsg);
}

/*!
 * \brief moves the current_char pointer and adds 1 to pos counter
 * 
 */
void Lexer::advance()
{
    this->pos++;
    cout << "Position: " << this->pos << endl;
    cout << "Current char: " << *(this->current_char) << endl;
    if ((unsigned int)this->pos >= this->text.length())
    {
        *(this->current_char) = '\0';
    }
    else
    {
        this->current_char++;
    }
}

/*!
 * \brief check the following char in the text string
 * 
 * \return a char* to the character next to the current_char, if the file is over then it assigns '\0' to the char 
 *
char *Lexer::peek()
{
    int peekPos = this->pos + 1;
    if ((unsigned int)peekPos >= this->text.length())
    {
        return '\0';
    }
    else
    {
        return this->current_char + 1;
    }
}*/

/**
 * \brief skips all spaces that appear together when called
 * 
 */
void Lexer::skipWhitespace()
{
    while (*(this->current_char) != '\0' && *(this->current_char) == ' ')
    {
        this->advance();
    }
}

/*!
 * \brief scans the text for an integer
 * 
 * \return string representing integer that was found in the text
 */
Token Lexer::findNumber()
{
    string result = "";
    while (*(this->current_char) != '\0' && isdigit(*(this->current_char)))
    {
        result += *(this->current_char);
        this->advance();
    }
    if (*(this->current_char) == '.')
    {
        result += *(this->current_char);
        this->advance();
        while (*(this->current_char) != '\0' && isdigit(*(this->current_char)))
        {
            result += *(this->current_char);
            this->advance();
        }
        return Token(DECIMAL, result);
    }
    else
    {
        return Token(INTEGER, result);
    }
}

/*!
 * \brief scans the text for a complete identifier
 * 
 * \return Token token type is ID, unless the ID was a reserved word, in which case the type is the reserved word
 */
Token Lexer::findId()
{
    string result = "";
    while (*(this->current_char) != '\0' && isalnum(*(this->current_char)))
    {
        result += *(this->current_char);
        this->advance();
    }

    if (RESERVED_KEYWORDS.find(result) == RESERVED_KEYWORDS.end())
    {
        return Token(ID, result);
    }
    else if (*(this->current_char) == '.')
    {
        return Token(STRUCTACCESS, result + "." + this->findId().getValue());
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
    string result = "";
    this->advance();
    if (*(this->current_char) != '\'')
    {
        if (*(this->current_char) == '\n' || *(this->current_char) == '\0') //Reached end of line or EOF without closing string
        {
            this->error("Unclosed char");
        }
        result += *(this->current_char);
        this->advance();
        if (*(this->current_char) != '\'')
        {
            this->error("Char too long");
        }
    }
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
    while (*(this->current_char) != '\0')
    {
        //cout << "Current char: " << *(this->current_char) << endl;
        if (*(this->current_char) == ' ')
        {
            this->skipWhitespace();
            continue;
        }
        if (*(this->current_char) == '\n')
        {
            this->line++;
            this->advance();
            return Token(EOL, EOL);
        }
        if (isalpha(*(this->current_char)))
        {
            return this->findId();
        }
        if (isdigit(*(this->current_char)))
        {
            return this->findNumber();
        }
        if (*(this->current_char) == '\'')
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
        string errorMsg = "Invalid character '" + *(this->current_char);
        this->error(errorMsg + "'");
    }
    return Token(EOFF, EOFF);
}