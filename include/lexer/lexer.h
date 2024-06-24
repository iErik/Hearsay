#pragma once

#include <stddef.h>

#include "common.h"
#include "tokens.h"


// -> Types
// --------

typedef struct lexer {
  cstring input;
  size_t inputLen;

  // The position of the last character
  int lastPosition;

  // Current position in input (points to current char)
  int position;

  // Current reading position in input (after current char)
  int readPosition;

  // Current character under examination
  char ch;
} lexer;


// -> Functions
// ------------

lexer* mkLexer (cstring input);
void destroyLexer (lexer* lex);

void readChar (lexer* lex);
char* readNumber (lexer* lex);
char* readIdentifier (lexer* lex);

char lookAhead (lexer* lex);
char lookBehind(lexer* lex);

token nextToken (lexer* lex);
void skipWhitespace (lexer* lex);

int reachedEOF(lexer* lex);

int isDigitSeparator (lexer* lex);
