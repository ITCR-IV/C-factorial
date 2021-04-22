#include "Token.h"

using namespace std;

/*!
 * \brief Construct a new Token object
 * 
 * \param type type of the token
 * \param value value for the token (for the lexer this is often same as type)
 */
Token::Token(string type, string value)
{
    this->type = type;
    this->value = value;
}

/*!
 * \brief get the type of the token
 * 
 * \return string 
 */
string Token::getType()
{
    return this->type;
}

/*!
 * \brief get the value of the token
 * 
 * \return string 
 */
string Token::getValue()
{
    return this->value;
}