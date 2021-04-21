#include "Lexer.h"
#include <cstring>
#include <string>
#include <ctype.h>

using namespace std;

/*!
 * @brief Construct a new Lexer object
 * 
 * @param text The entire code text
 */
Lexer::Lexer(string text)
{
    this->text = text;
    this->pos = 0;
    this->current_char = &text[this->pos];
    this->line = 1;
}

/*!
 * @brief The lexer throws a SyntaxException if it can't tokenize properly
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
 * @brief moves the current_char pointer and adds 1 to pos counter
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
 * @brief check the following char in the text string
 * 
 * @return a char* to the character next to the current_char, if the file is over then it returns nullptr 
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
 * @brief skips all spaces that appear together when called
 * 
 */
void Lexer::skipWhitespace()
{
    while (current_char != nullptr && *current_char == ' ')
    {
        this->advance();
    }
}

/*!
 * @brief scans the text for an integer
 * 
 * @return int that was found in the text
 */
int Lexer::findInteger()
{
    string result = "";
    while (this->current_char != nullptr && isdigit(*(this->current_char)))
    {
        result += *(this->current_char);
        this->advance();
    }
    return stoi(result);
}

string Lexer::findId()
{
}

Token Lexer::getNextToken()
{
}