#pragma once

#include <stddef.h>
#include "lexer/tokens.h"

// ----------------------------------------------------- //
// -> Macros                                             //
// ----------------------------------------------------- //

#define AsString(node) _Generic((node),                   \
    identifierNode*: identifierAsString,                  \
    letStatement*: letAsString,                           \
    retStatement*: returnAsString,                        \
    rootNode*: rootAsString,                              \
    invalidNode*: invalidAsString,                        \
    nodeWrapper*: wrapperAsString                         \
  )(node)

#define TokenLiteral(node) _Generic((node),               \
    rootNode: rootTokenLiteral                            \
  )(&node)

#define FreeNode(node) _Generic((node),                   \
    nodeWrapper: unmkNodeWrapper,                         \
    rootNode*: unmkRootNode,                              \
    invalidNode*: unmkInvalidNode,                        \
    identifierNode*: unmkIdNode,                          \
    blockStatement*: unmkBlockNode,                       \
    letStatement*: unmkLetNode,                           \
    retStatement*: unmkReturnNode,                        \
    prefixExpr*: unmkPrefixNode,                          \
    infixExpr*: unmkInfixNode,                            \
    ifExpr*: unmkIfNode,                                  \
    boolExpr*: unmkBoolNode,                              \
    fnExpr*: unmkFnNode,                                  \
    integerLiteral*: unmkIntNode                          \
  )(&node)

#define WrapNode(node) _Generic((node),                   \
    rootNode*: wrapRootNode,                              \
    identifierNode*: wrapIdentifierNode,                  \
    letStatement*: wrapLetNode,                           \
    retStatement*: wrapReturnNode,                        \
    exprStatement*: wrapExprNode,                         \
    invalidNode*: wrapInvalidNode                         \
  )(node)

#define INVALID_STATEMENT (nodeWrapper) {                 \
  .type = InvalidStatement,                               \
  .node = NULL                                            \
}

// ----------------------------------------------------- //
// -> Enums / Constants                                  //
// ----------------------------------------------------- //

typedef enum {
  RootNode,

  IdentifierNode,
  ExpressionGroupNode,

  BooleanLiteral,
  IntegerLiteral,
  InfixExpression,

  FunctionExpression,
  IfExpression,
  PrefixExpression,
  BooleanExpression,

  LetStatement,
  ReturnStatement,
  BlockStatement,
  InvalidStatement
} nodeType;


// ----------------------------------------------------- //
// -> NodeList                                           //
// ----------------------------------------------------- //

#define NODE_LIST_INIT_CAP 5

typedef struct nodeWrapper nodeWrapper;

typedef struct nodeList {
  nodeWrapper* nodes;
  nodeWrapper* offsetPtr;

  size_t       length;
  size_t       capacity;
  size_t       iteratorPos;
} nodeList;

nodeList*    nl_new      ();
nodeWrapper* nl_get      (nodeList* nList, uint offset);
nodeWrapper* nl_nextNode (nodeList* nList);
nodeWrapper* nl_iterator (nodeList* nList);

void  nl_destroy       (nodeList** nList);
void* nl_grow          (nodeList* nList);
void  nl_push          (nodeList* nList, nodeWrapper node);
void  nl_resetIterator (nodeList* nList);

// ----------------------------------------------------- //
// -> Structs / Types                                    //
// ----------------------------------------------------- //

typedef struct nodeWrapper {
  void*    node;
  nodeType type;
} nodeWrapper;

typedef struct rootNode {
  nodeList* nodes;
} rootNode;

typedef struct invalidNode {
  token token;
} invalidNode;

typedef struct identifierNode {
  token   token;
  cstring value;
} identifierNode;

typedef struct blockStatement {
  nodeList* statements;
} blockStatement;

typedef struct exprStatement {
  token       token;
  nodeWrapper expression;
} exprStatement;

typedef struct letStatement {
  token            token;
  identifierNode*  name;
  nodeWrapper      value;
} letStatement;

typedef struct retStatement {
  token       token;
  nodeWrapper value;
} retStatement;

typedef struct prefixExpr {
  token       token;
  cstring     operator;
  nodeWrapper right;
} prefixExpr;

typedef struct infixExpr {
  token       token;
  nodeWrapper left;
  cstring     operator;
  nodeWrapper right;
} infixExpr;

typedef struct ifExpr {
  token       token;
  nodeWrapper condition;
  nodeWrapper body;
  nodeWrapper elseBlock;
} ifExpr;

typedef struct boolExpr {
  token token;
  unsigned char value;
} boolExpr;

typedef struct fnExpr {
  token       token;
  nodeWrapper params;
  nodeWrapper body;
} fnExpr;

typedef struct integerLiteral {
  token  token;
  double value;
} integerLiteral;


// ----------------------------------------------------- //
// -> Functions                                          //
// ----------------------------------------------------- //

rootNode* mkRootNode ();
void* growRootNode (rootNode* rNode);
void pushNode (rootNode* rNode, nodeWrapper node);
nodeWrapper* getNode (rootNode* rNode, uint offset);
nodeWrapper* nextNode (rootNode* rNode);
nodeWrapper* iterator (rootNode* rNode);
void resetIterator (rootNode* rNode);


letStatement* mkLetStatement (token id, nodeWrapper val);
retStatement* mkRetStatement (nodeWrapper val);
identifierNode* mkIdNode (token tkn);
invalidNode* mkInvalidNode (token tkn);


cstring rootAsString (rootNode* rNode);
cstring letAsString (letStatement* node);
cstring returnAsString (retStatement* node);
cstring identifierAsString (identifierNode* node);
cstring invalidAsString (invalidNode* node);
cstring wrapperAsString (nodeWrapper* node);


cstring rootTokenLiteral (rootNode* rNode);
cstring getTokenLiteral (nodeWrapper* wrapper);


nodeWrapper wrapRootNode (rootNode* node);
nodeWrapper wrapIdentifierNode (identifierNode* node);
nodeWrapper wrapExprNode (void* node);
nodeWrapper wrapLetNode (letStatement* node);
nodeWrapper wrapReturnNode (retStatement* node);
nodeWrapper wrapInvalidNode (invalidNode* node);


void unmkRootNode (rootNode** node);
void unmkLetNode (letStatement** node);
void unmkIdNode (identifierNode** node);
void unmkReturnNode (retStatement** node);
void unmkBlockNode (blockStatement** node);
void unmkPrefixNode (prefixExpr** node);
void unmkInfixNode (infixExpr** node);
void unmkIfNode (ifExpr** node);
void unmkBoolNode (boolExpr** node);
void unmkFnNode (fnExpr** node);
void unmkIntNode (integerLiteral** node);
void unmkInvalidNode (invalidNode** node);
void unmkNodeWrapper (nodeWrapper* node);
