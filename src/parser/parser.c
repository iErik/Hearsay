#include "utils/array.h"
#include "utils/text.h"
#include "utils/mem.h"

#include "lexer/tokens.h"
#include "parser/nodes.h"
#include "parser/parser.h"


nodeWrapper parseExpression (
  parser* pars, opPrecedence prec);

// Prefix Fns
nodeWrapper parseIdentifier (parser* pars);
nodeWrapper parseIntegerLiteral (parser* pars);
nodeWrapper parsePrefix (parser* pars);
nodeWrapper parseInfix (parser* pars, nodeWrapper left);
nodeWrapper parseGroupedExpression (parser* pars);

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
  registerPrefixFn(pars, TknLParen, parseGroupedExpression);

  registerInfixFn(pars, TknPlus, parseInfix);
  registerInfixFn(pars, TknMinus, parseInfix);
  registerInfixFn(pars, TknSlash, parseInfix);
  registerInfixFn(pars, TknAsterisk, parseInfix);
  registerInfixFn(pars, TknEQ, parseInfix);
  registerInfixFn(pars, TknNEQ, parseInfix);
  registerInfixFn(pars, TknLT, parseInfix);
  registerInfixFn(pars, TknGT, parseInfix);

  // Fix it;
  advanceParser(pars);
  advanceParser(pars);

  return pars;
}

void advanceParser (parser* pars) {
  pars->lastToken = pars->currToken;
  pars->currToken = pars->peekToken;
  pars->peekToken = nextToken(pars->lexer);
}

nodeList* parse (cstring input) {
  lexer* lex = mkLexer(input);
  parser* pars = mkParser(lex);

  rootNode* rNode = parseProgram(pars);
  nodeList* nodes = rNode->nodes;

  sFree(&rNode);

  return nodes;
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
// -> Node Parsing                                       //
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
  letStatement* letStm = make(letStatement);

  letStm->token = pars->currToken;

  if (!peekExpect(pars, TknIdent))
    return INVALID_STATEMENT;

  letStm->name = mkIdNode(pars->currToken);

  if (!peekExpect(pars, TknAssign))
    return INVALID_STATEMENT;

  advanceParser(pars);

  letStm->value = parseExpression(pars, PrecLowest);

  if (peekTknIs(pars, TknSemicolon))
    advanceParser(pars);

  return (nodeWrapper) {
    .type = LetStatement,
    .node = letStm
  };
}

nodeWrapper parseRetStatement (parser* pars) {
  retStatement* retState = make(retStatement);
  retState->token = pars->currToken;

  advanceParser(pars);

  retState->value = parseExpression(pars, PrecLowest);

  while (!tokenIs(pars->currToken, TknSemicolon))
    advanceParser(pars);

  return (nodeWrapper) {
    .type = ReturnStatement,
    .node = retState
  };
}

void noPrefixParseFnError (parser* pars, tokenType tkn) {
  pushParserError(pars, interpol(
    "No prefix parse function for %d found", tkn));
}

// ----------------------------------------------------- //
// -> Expressions                                        //
// ----------------------------------------------------- //

nodeWrapper parseExpression (
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

nodeWrapper parseExprStatement (parser* pars) {
  return parseExpression(pars, PrecLowest);
}

// ----------------------------------------------------- //
// -> Block Statements                                   //
// ----------------------------------------------------- //

nodeWrapper parseBlockStatement (parser* pars) {
  blockStatement* block = make(blockStatement);
  advanceParser(pars);

  while (!tokenIs(pars->currToken, TknRBrace)) {
    nodeWrapper node = parseStatement(pars);
    advanceParser(pars);

    nl_push(block->statements, node);
  }

  advanceParser(pars);

  return (nodeWrapper) {
    .type = BlockStatement,
    .node = block
  };
}

// ----------------------------------------------------- //
// -> Prefix Expression                                  //
// ----------------------------------------------------- //

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

// ----------------------------------------------------- //
// -> Infix                                              //
// ----------------------------------------------------- //

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

// ----------------------------------------------------- //
// -> Grouped Epxressions (Left Parent)                  //
// ----------------------------------------------------- //

nodeWrapper parseGroupedExpression (parser* pars) {
  advanceParser(pars);

  nodeWrapper expr = parseExpression(pars, PrecLowest);

  if (peekExpect(pars, TknRParen))
    return INVALID_STATEMENT;

  return expr;
}

// ----------------------------------------------------- //
// -> If Expressions                                     //
// ----------------------------------------------------- //

nodeWrapper parseIfExpr (parser* pars) {
  ifExpr* expr = make(ifExpr);

  expr->token = pars->currToken;

  if (!peekExpect(pars, TknLParen))
    return INVALID_STATEMENT;

  advanceParser(pars);

  expr->condition = parseExpression(pars, PrecLowest);

  if (!peekExpect(pars, TknRParen))
    return INVALID_STATEMENT;

  if (!peekTknIs(pars, TknLBrace))
    return INVALID_STATEMENT;

  expr->body = parseBlockStatement(pars);

  if (currTknIs(pars, TknElse)) {
    advanceParser(pars);
    expr->elseBlock = parseBlockStatement(pars);
  }

  return (nodeWrapper) {
    .type = IfExpression,
    .node = expr
  };
}

// ----------------------------------------------------- //
// -> Identifiers                                        //
// ----------------------------------------------------- //

nodeWrapper parseIdentifier (parser* pars) {
  return WrapNode(mkIdNode(pars->currToken));
}

// ----------------------------------------------------- //
// -> Integer Literals                                   //
// ----------------------------------------------------- //

nodeWrapper parseIntegerLiteral (parser* pars) {
  integerLiteral* lit = make(integerLiteral);

  lit->token = pars->currToken;
  lit->value = atoi(pars->currToken.literal);

  return (nodeWrapper) {
    .type = IntegerLiteral,
    .node = lit
  };
}

// ----------------------------------------------------- //
// -> Function Literal                                   //
// ----------------------------------------------------- //

nodeWrapper parseFunctionLiteral (parser* pars) {
  fnExpr* expr = make(fnExpr);

  expr->token = pars->currToken;

  if (!peekExpect(pars, TknLParen))
    return INVALID_STATEMENT;

  expr->params = parseExpression(pars, PrecLowest);

  if (!peekExpect(pars, TknRParen))
    return INVALID_STATEMENT;

  if (!peekTknIs(pars, TknLBrace))
    return INVALID_STATEMENT;

  expr->body = parseExpression(pars, PrecLowest);

  return (nodeWrapper) {
    .type = FunctionExpression,
    .node = expr
  };
}

// ----------------------------------------------------- //
// -> Boolean Literal                                    //
// ----------------------------------------------------- //

nodeWrapper parseBooleanLiteral (parser* pars) {
  boolExpr* lit = make(boolExpr);

  lit->token = pars->currToken;
  lit->value = currTknIs(pars, TknTrue);

  return (nodeWrapper) {
    .type = BooleanLiteral,
    .node = lit
  };
}
