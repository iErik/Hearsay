#pragma once

#include "utils/array.h"
#include "lexer/lexer.h"
#include "lexer/tokens.h"

#include "parser/nodes.h"


// ----------------------------------------------------- //
// -> Structs / Types                                    //
// ----------------------------------------------------- //

struct parser;

typedef nodeWrapper (*prefixParserFn)(struct parser* pars);
typedef nodeWrapper (*infixParserFn)(
    struct parser* pars,
    nodeWrapper leftNode);

typedef struct parser {
  lexer* lexer;
  array* errors;

  token lastToken;
  token currToken;
  token peekToken;

  prefixParserFn prefixParserFns[TKN_LEN];
  infixParserFn  infixParserFns[TKN_LEN];
} parser;


// ----------------------------------------------------- //
// -> Functions                                          //
// ----------------------------------------------------- //

parser* mkParser (lexer* lex);

void advanceParser (parser* pars);

void peekError (parser* pars, tokenType expectedType);

bool currTknIs (parser* pars, tokenType type);

bool peekTknIs (parser* pars, tokenType type);

bool peekExpect (parser* pars, tokenType type);

bool hasParsingErrors (parser* pars);

rootNode* parseProgram (parser* pars);

cstring listParserErrors (parser* pars);

void registerPrefixFn (
  parser* pars,
  tokenType token,
  prefixParserFn fn);
void registerInfixFn (
  parser* pars,
  tokenType token,
  infixParserFn fn);

// ?

nodeWrapper parseStatement (parser* pars);
nodeWrapper parseLetStatement (parser* pars);
nodeWrapper parseRetStatement (parser* pars);
nodeWrapper parseExprStatement (parser* pars);

//nodeWrapper parseExpression (parser* pars);
