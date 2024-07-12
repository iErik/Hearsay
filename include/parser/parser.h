#pragma once

#include "utils/array.h"
#include "lexer/lexer.h"
#include "lexer/tokens.h"

#include "parser/nodes.h"

typedef struct parser {
  lexer* lexer;
  array* errors;

  token lastToken;
  token currToken;
  token peekToken;
} parser;

parser* mkParser (lexer* lex);

void advanceParser (parser* pars);

void peekError (parser* pars, tokenType expectedType);

bool currTknIs (parser* pars, tokenType type);

bool peekTknIs (parser* pars, tokenType type);

bool peekExpect (parser* pars, tokenType type);

bool hasParsingErrors (parser* pars);

rootNode* parseProgram (parser* pars);

nodeWrapper parseLetStatement (parser* pars);

nodeWrapper parseStatement (parser* pars);

cstring listParserErrors (parser* pars);
