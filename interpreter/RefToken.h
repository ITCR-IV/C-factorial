#ifndef REFTOKEN_H
#define REFTOKEN_H

#include "Token.h"
#include "TokenConstants.h"
#include <string>

using namespace std;

//! class reftoken indicates the reference to a token object
class RefToken : public Token
{
private:
    //! the type that the pointer points to
    string refType;

public:
    /*!
     * \brief Construct a new Ref Token object
     * 
     * \param refType type that the reference points to
     * \param value address of pointer
     */
    RefToken(string refType, string value) : Token(REFERENCE, value)
    {
        this->refType = refType;
    }
};

#endif