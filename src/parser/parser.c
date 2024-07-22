#include "utils/array.h"
#include "utils/text.h"

#include "lexer/tokens.h"
#include "parser/nodes.h"
#include "parser/parser.h"


// Prefix Fns
nodeWrapper parseIdentifier (parser* pars);
nodeWrapper parseIntegerLiteral (parser* pars);
nodeWrapper parsePrefix (parser* pars);
nodeWrapper parseInfix (parser* pars, nodeWrapper left);

// ----------------------------------------------------- //
// -> Parser                                             //
// ----------------------------------------------------- //

parser* mkParser (lexer* lex) {
  parser* pars = cmake(parser);
  checkNullPtr(pars);

  pars->errors = mkArray();
  pars->lexer = lex;

  registerPrefixFn(pars, TknIdent, parseIdentifier);
  registerPrefixFn(pars, TknInt, parseIntegerLiteral);
  registerPrefixFn(pars, TknPlus, parsePrefix);
  registerPrefixFn(pars, TknMinus, parsePrefix);
  registerPrefixFn(pars, TknBang, parsePrefix);

  registerInfixFn(pars, TknPlus, parseInfix);
  registerInfixFn(pars, TknMinus, parseInfix);
  registerInfixFn(pars, TknSlash, parseInfix);
  registerInfixFn(pars, TknAsterisk, parseInfix);
  registerInfixFn(pars, TknEQ, parseInfix);
  registerInfixFn(pars, TknNEQ, parseInfix);
  registerInfixFn(pars, TknLT, parseInfix);
  registerInfixFn(pars, TknGT, parseInfix);

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

void peekError (parser* pars, tokenType expectedType) {
  cstring error = interpol(
    "Expected next token to be %i, got %i instead",
    expectedType, pars->peekToken.type);

  arrPush(pars->errors, error);
}

void pushParserError (parser* pars, cstring message) {
  arrPush(pars->errors, message);
}

opPrecedence peekPrecedence (parser* pars) {
  return tknPrecedence(pars->peekToken);
}

opPrecedence currPrecedence (parser* pars) {
  return tknPrecedence(pars->currToken);
}

bool hasParsingErrors (parser* pars) {
  return arrLen(pars->errors) > 0;
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

rootNode* parseProgram (parser* pars) {
  rootNode* root = mkRootNode();
  checkNullPtr(root);

  while (!tokenIs(pars->currToken, TknEOF)) {
    nodeWrapper node = parseStatement(pars);
    advanceParser(pars);

    if (node.type != InvalidStatement)
      pushNode(root, node);
  }

  return root;
}

void registerPrefixFn (
  parser* parser,
  tokenType token,
  prefixParserFn fn
) { parser->prefixParserFns[token] = fn; }

void registerInfixFn (
  parser* parser,
  tokenType token,
  infixParserFn fn
) { parser->infixParserFns[token] = fn; }


// ----------------------------------------------------- //
// -> Parser                                             //
// ----------------------------------------------------- //

nodeWrapper parseStatement (parser* pars) {
  switch (pars->currToken.type) {
    case TknLet:
      return parseLetStatement(pars);
    case TknReturn:
      return parseRetStatement(pars);
    default:
      return parseExprStatement(pars);
  }

  return INVALID_STATEMENT;
}

nodeWrapper parseLetStatement (parser* pars) {
  token letTkn = pars->currToken;

  if (!peekExpect(pars, TknIdent))
    return INVALID_STATEMENT;

  token idTkn = pars->currToken;

  if (!peekExpect(pars, TknAssign))
    return INVALID_STATEMENT;

  letStatement* letNode = mkLetStatement(letTkn, idTkn);

  while (!tokenIs(pars->currToken, TknSemicolon))
    advanceParser(pars);

  return WrapNode(letNode);
}

nodeWrapper parseRetStatement (parser* pars) {
  token retTkn = pars->currToken;

  advanceParser(pars);

  while (!tokenIs(pars->currToken, TknSemicolon))
    advanceParser(pars);

  retStatement* retStat = mkRetStatement(retTkn);

  return WrapNode(retStat);
}

void noPrefixParseFnError (parser* pars, tokenType tkn) {
  pushParserError(pars, interpol(
    "No prefix parse function for %s found", tkn));
}

exprWrapper parseExpression (
  parser* pars,
  opPrecedence precedence
) {
  token tkn = pars->currToken;
  tokenType tknType = tkn.type;

  prefixParserFn prefixFn = pars->prefixParserFns[tknType];

  if (prefixFn == NULL) {
    noPrefixParseFnError(pars, tknType);
    return WrapNode(mkInvalidNode(tkn));
  }

  nodeWrapper leftExpr = prefixFn(pars);

  while (!peekTknIs(pars, TknSemicolon) &&
          precedence < peekPrecedence(pars))
  {
    infixParserFn infixFn =
      pars->infixParserFns[pars->peekToken.type];

    if (infixFn == NULL)
      return leftExpr;

    advanceParser(pars);

    leftExpr = infixFn(pars, leftExpr);
  }

  return leftExpr;
}

nodeWrapper parsePrefix (parser* pars) {
  prefixExpr* node = make(prefixExpr);

  node->token      = pars->currToken;
  node->operator   = pars->currToken.literal;

  advanceParser(pars);
  node->right = parseExpression(pars, PrecPrefix);

  return (nodeWrapper) {
    .type = PrefixExpression,
    .node = node
  };
}

nodeWrapper parseInfix (parser* pars, nodeWrapper left) {
  infixExpr* node = make(infixExpr);

  node->token     = pars->currToken;
  node->operator  = pars->currToken.literal;
  node->left      = left;

  opPrecedence precedence = currPrecedence(pars);
  advanceParser(pars);
  node->right = parseExpression(pars, precedence);

  return (nodeWrapper) {
    .type = InfixExpression,
    .node = node
  };
}

nodeWrapper parseIfExpr (parser* pars) { }

nodeWrapper parseIdentifier (parser* pars) {
  return WrapNode(mkIdNode(pars->currToken));
}

nodeWrapper parseIntegerLiteral (parser* pars) {
  integerLiteral* lit = make(integerLiteral);

  lit->token = pars->currToken;
  lit->value = atoi(pars->currToken.literal);

  return (nodeWrapper) {
    .type = IntegerLiteral,
    .node = lit
  };
}

nodeWrapper parseBlockStatement (parser* pars) { }

nodeWrapper parseFunctionLiteral (parser* pars) { }

nodeWrapper parseBooleanLiteral (parser* pars) { }

nodeWrapper parseExprStatement (parser* pars) {
  exprStatement* node = make(exprStatement);

  node->token = pars->currToken;
  node->expression = parseExpression(pars, PrecLowest);

  if (peekTknIs(pars, TknSemicolon))
    advanceParser(pars);

  return (nodeWrapper) {
    .type = ExpressionNode,
    .node = node
  };
}

