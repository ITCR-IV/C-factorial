#ifndef TOKEN_CONSTANTS_H
#define TOKEN_CONSTANTS_H

#include <string>
#include <unordered_map>
#include "Token.h"

// token types
const std::string EOFF = "EOFF";
const std::string INTEGER = "INTEGER";
const std::string DECIMAL = "DECIMAL";
const std::string STRING = "STRING";
const std::string ID = "ID";
const std::string STRUCTACCESS = "STRUCTACCESS";

// symbols (also token types)
const std::string LBRACK = "{";
const std::string RBRACK = "}";
const std::string LPAREN = "(";
const std::string RPAREN = ")";
const std::string LARROW = "<";
const std::string RARROW = ">";
const std::string EQUAL = "=";
const std::string DOT = ".";
const std::string SEMI = ";";
const std::string PLUS = "+";
const std::string MINUS = "-";
const std::string MUL = "*";
const std::string DIV = "/";

// reserved words (also token types)
const std::string STRUCT = "struct";
const std::string INT = "int";
const std::string LONG = "long";
const std::string CHAR = "char";
const std::string FLOAT = "float";
const std::string DOUBLE = "double";
const std::string REFERENCE = "reference";
const std::string GETADDR = "getAddr";
const std::string GETVALUE = "getValue";
const std::string PRINT = "print";

const std::unordered_map<std::string, std::string> RESERVED_KEYWORDS = {
    {STRUCT, STRUCT}, {INT, INT}, {LONG, LONG}, {CHAR, CHAR}, {FLOAT, FLOAT}, {DOUBLE, DOUBLE}, {REFERENCE, REFERENCE}, {GETADDR, GETADDR}, {GETVALUE, GETVALUE}, {PRINT, PRINT}};

#endif