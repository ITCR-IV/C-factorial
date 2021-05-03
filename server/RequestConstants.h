#ifndef TYPE_CONSTANTS
#define TOKEN_CONSTANTS_H

const int ERROR                 = -1;
const int SUCCESS               = 0;
const int ENTERSCOPE            = 1;
const int EXITSCOPE             = 2;
const int DECLARE               = 3;
const int DECLAREREF            = 4;
const int CHANGEVALUE           = 5;
const int ENTERSTRUCT           = 6;
const int EXITSTRUCT            = 7;
const int REQUESTVARIABLE       = 8;
const int REQUESTMEMORYSTATE    = 9;
const int FLUSH                 = 10;
const int REQUESTBYADDRESS      = 11;

const int REQUESTUPPERBOUND     = 12; //This isn't a real request but it's used to keep track of the maximum possible value for a request, if new requests added then update this 

#endif