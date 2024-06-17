#pragma once

#include "lexer.h"
#include "tokens.h"

#include "parser/nodes.h"

typedef struct parser {
  lexer* lexer;

  token lastToken;
  token currToken;
  token peekToken;
} parser;

parser* mkParser (lexer* lex);

void advanceParser (parser* pars);

bool currTknIs (parser* pars, tokenType type);

bool peekTknIs (parser* pars, tokenType type);

bool peekExpect (parser* pars, tokenType type);

programNode* parseProgram (parser* pars);

statementNode parseLetStatement (parser* pars);
//statementNode* parseLetStatement (parser* pars);

statementNode parseStatement (parser* pars);
//statementNode* parseStatement (parser* pars);
