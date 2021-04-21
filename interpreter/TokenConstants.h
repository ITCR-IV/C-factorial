#ifndef TOKEN_CONSTANTS_H
#define TOKEN_CONSTANTS_H

#include <string>
#include <unordered_map>
#include "Token.h"

// token types
const std::string EOFF = "EOFF";
const std::string INTEGER = "INTEGER";
const std::string STRING = "STRING";
const std::string ID = "ID";

// symbols
const std::string LBRACK = "{";
const std::string RBRACK = "}";
const std::string LPAREN = "(";
const std::string RPAREN = ")";
const std::string LARROW = "<";
const std::string RARROW = ">";
const std::string EQUAL = "=";
const std::string DOT = ".";
const std::string SEMI = ";";
const std::string PLUS = "PLUS";
const std::string MINUS = "MINUS";
const std::string MUL = "MUL";
const std::string DIV = "DIV";

// reserved words
const std::string STRUCT = "STRUCT";
const std::string INT = "INT";
const std::string LONG = "LONG";
const std::string CHAR = "CHAR";
const std::string FLOAT = "FLOAT";
const std::string DOUBLE = "DOUBLE";
const std::string REFERENCE = "REFERENCE";
const std::string GETADDR = "GETADDR";
const std::string GETVALUE = "GETVALUE";

const std::unordered_map<std::string, std::string> RESERVED_KEYWORDS = {
    {STRUCT, STRUCT}, {INT, INT}, {LONG, LONG}, {CHAR, CHAR}, {FLOAT, FLOAT}, {DOUBLE, DOUBLE}, {REFERENCE, REFERENCE}, {GETADDR, GETADDR}, {GETVALUE, GETVALUE}};

#endif