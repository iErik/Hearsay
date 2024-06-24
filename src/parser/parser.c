#include "utils/array.h"
#include "utils/text.h"

#include "lexer/tokens.h"
#include "parser/nodes.h"
#include "parser/parser.h"


parser* mkParser (lexer* lex) {
  parser* pars = cmake(parser);
  checkNullPtr(pars);

  pars->errors = mkArray();
  pars->lexer = lex;

  return pars;
}

void advanceParser (parser* pars) {
  pars->lastToken = pars->currToken;
  pars->currToken = pars->peekToken;
  pars->peekToken = nextToken(pars->lexer);
}

bool currTknIs (parser* pars, tokenType type) {
  return tokenIs(pars->currToken, type);
}

bool peekTknIs (parser* pars, tokenType type) {
  return tokenIs(pars->peekToken, type);
}

bool peekExpect (parser* pars, tokenType type) {
  if (!peekTknIs(pars, type)) {
    peekError(pars, type);

    return false;
  } else {
    advanceParser(pars);

    return true;
  }
}

cstring listParserErrors (parser* pars) {
  if (!hasParsingErrors(pars)) return "";

  cstring errors = "\n";

  for (int i = 0; i < arrLen(pars->errors); i++) errors =
    interpol(
      "%s%s\n",
      errors,
      (cstring) arrGet(pars->errors, i));

  return errors;
}

bool hasParsingErrors (parser* pars) {
  return arrLen(pars->errors) > 0;
}

void peekError (parser* pars, tokenType expectedType) {
  cstring error = interpol(
    "Expected next token to be %i, got %i instead",
    expectedType, pars->peekToken.type);

  arrPush(pars->errors, error);
}

parserNode* parseProgram (parser* pars) {
  parserNode* root = mkRootNode();
  checkNullPtr(root);

  while (!tokenIs(pars->currToken, TknEOF)) {
    parserNode node = parseStatement(pars);
    advanceParser(pars);

    if (node.type != InvalidStatement)
      pushNode(root, node);
  }

  return root;
}

parserNode parseLetStatement (parser* pars) {
  token letTkn = pars->currToken;

  if (!peekExpect(pars, TknIdent))
    return INVALID_STATEMENT;

  token idTkn = pars->currToken;

  if (!peekExpect(pars, TknAssign))
    return INVALID_STATEMENT;

  parserNode letNode = mkLetStatement(letTkn, idTkn);

  while (!tokenIs(pars->currToken, TknSemicolon))
    advanceParser(pars);

  return letNode;
}

parserNode parseRetStatement (parser* pars) {
  token retTkn = pars->currToken;

  advanceParser(pars);

  while (!tokenIs(pars->currToken, TknSemicolon))
    advanceParser(pars);

  parserNode retStat = mkRetStatement(retTkn);

  return retStat;
}

parserNode parseStatement (parser* pars) {
  switch (pars->currToken.type) {
    case TknIllegal:
      break;
    case TknEOF:
      break;
    case TknIdent:
      break;
    case TknInt:
      break;
    case TknAssign:
      break;
    case TknPlus:
      break;
    case TknMinus:
      break;
    case TknAsterisk:
      break;
    case TknSlash:
      break;
    case TknBang:
      break;
    case TknQuestion:
      break;
    case TknLT:
      break;
    case TknGT:
      break;
    case TknEQ:
      break;
    case TknNEQ:
      break;
    case TknBackslash:
      break;
    case TknComma:
      break;
    case TknSemicolon:
      break;
    case TknLParen:
      break;
    case TknRParen:
      break;
    case TknLBrace:
      break;
    case TknRBrace:
      break;
    case TknLet:
      return parseLetStatement(pars);
    case TknFunction:
      break;
    case TknReturn:
      return parseRetStatement(pars);
    case TknIf:
      break;
    case TknElif:
      break;
    case TknElse:
      break;
    case TknTrue:
      break;
    case TknFalse:
      break;
  }

  return INVALID_STATEMENT;
}
