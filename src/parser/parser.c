#include <stdlib.h>

#include "tokens.h"
#include "parser/nodes.h"
#include "parser/parser.h"


parser* mkParser (lexer* lex) {
  parser* pars = (parser*) calloc(1, sizeof(parser));
  checkNullPtr(pars);

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
  if (!peekTknIs(pars, type)) return false;

  advanceParser(pars);
  return true;
}

programNode* parseProgram (parser* pars) {
  programNode* program = (programNode *) calloc(1,
    sizeof(programNode));
  postMalloc(program);
  program->statements = mkNodeList();

  while (!tokenIs(pars->currToken, TknEOF)) {
    statementNode stat = parseStatement(pars);
    advanceParser(pars);

    if (stat.type != InvalidStatement)
      pushNode(program->statements, stat);
  }

  return program;
}

statementNode parseLetStatement (parser* pars) {
  letStatement* fields = (letStatement *) malloc(
    sizeof(letStatement));
  //{ .token = pars->currToken };
  //postMalloc(fields);

  fields->token = pars->currToken;

  if (!peekExpect(pars, TknIdent))
    return INVALID_STATEMENT;

  if (!peekExpect(pars, TknAssign))
    return INVALID_STATEMENT;

  fields->name = mkIdNode(
    pars->currToken,
    pars->currToken.literal
  );

  statementNode stat = mkLetStatement(fields);
  return stat;
}

statementNode parseStatement (parser* pars) {
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
      break;
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
