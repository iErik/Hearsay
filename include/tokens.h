#pragma once

#include "common.h"

typedef enum {
  TknIllegal,
  TknEOF,

  // Identifiers
  TknIdent,
  TknInt,

  // Literals / Operators
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
  TknFalse
} tokenType;

//typedef const char* tokenType;

typedef struct token {
  tokenType type;
  cstring literal;
} token;


token newToken (tokenType type, char* literal);
tokenType lookupIdent (cstring ident);

int isWhitespace (char ch);
int isLetter (char ch);
int isDigit (char ch);
int tokenIs (token tkn, tokenType type);
