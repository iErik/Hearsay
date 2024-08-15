#pragma once

#include "common.h"


// ----------------------------------------------------- //
// -> Enums / Constants                                  //
// ----------------------------------------------------- //

typedef enum {
  TknIllegal = 0,
  TknEOF,

  // Identifiers
  TknIdent,
  TknInt,

  // Operators
  TknAssign,
  TknPlus,
  TknMinus,
  TknAsterisk,
  TknSlash,

  TknBang,
  TknQuestion,

  TknLT,
  TknGT,
  TknEQ,
  TknNEQ,

  // Delimiters
  TknBackslash,
  TknComma,
  TknSemicolon,

  TknLParen,
  TknRParen,

  TknLBrace,
  TknRBrace,

  // Keywords
  TknLet,
  TknFunction,
  TknReturn,

  TknIf,
  TknElif,
  TknElse,

  TknTrue,
  TknFalse,

  TKN_LEN
} tokenType;

typedef enum {
  PrecLowest = 0,
  // ==
  PrecEquals,
  // > or <
  PrecLessGreater,
  // +
  PrecSum,
  // *
  PrecProduct,
  // -X or !X
  PrecPrefix,
  // myFunction(X)
  PrevCall,
} opPrecedence;


// ----------------------------------------------------- //
// -> Structs / Types                                    //
// ----------------------------------------------------- //

typedef struct token {
  tokenType type;
  cstring literal;
} token;


// ----------------------------------------------------- //
// -> Functions                                          //
// ----------------------------------------------------- //

token newToken (tokenType type, char* literal);
tokenType lookupIdent (cstring ident);

bool isWhitespace (char ch);
bool isLetter (char ch);
bool isDigit (char ch);
bool tokenIs (token tkn, tokenType type);
bool tokenEq (token* left, token* right);

opPrecedence tknPrecedence (token tkn);
