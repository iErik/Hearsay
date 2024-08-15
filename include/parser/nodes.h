#pragma once

#include <stddef.h>
#include "lexer/tokens.h"

// ----------------------------------------------------- //
// -> Macros                                             //
// ----------------------------------------------------- //

#define AsString(node) _Generic((node),                   \
    nodeWrapper*: wrapperAsString,                        \
    rootNode*: rootAsString,                              \
    invalidNode*: invalidAsString,                        \
    identifierNode*: identifierAsString,                  \
    blockStatement*: blockAsStr,                          \
    letStatement*: letAsString,                           \
    retStatement*: returnAsString,                        \
    prefixExpr*: prefixAsStr,                             \
    infixExpr*: infixAsStr,                               \
    ifExpr*: ifAsStr,                                     \
    boolExpr*: boolAsStr,                                 \
    fnExpr*: fnAsStr,                                     \
    integerLiteral*: intAsStr                             \
  )(node)

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
    invalidNode*: wrapInvalidNode,                        \
    identifierNode*: wrapIdentifierNode,                  \
    blockStatement*: wrapBlock,                           \
    letStatement*: wrapLetNode,                           \
    retStatement*: wrapReturnNode,                        \
    prefixExpr*: wrapPrefixNode,                          \
    infixExpr*: wrapInfixNode,                            \
    ifExpr*: wrapIfNode,                                  \
    boolExpr*: wrapBoolNode,                              \
    fnExpr*: wrapFnNode,                                  \
    integerLiteral*: wrapIntNode                          \
  )(node)

#define CompareNodes(left, right) _Generic((left),        \
    nodeWrapper*: compareNodeWrapper,                     \
    rootNode*: compareRoot,                               \
    letStatement*: compareLet,                            \
    identifierNode*: compareId,                           \
    fnExpr*: compareFn,                                   \
    blockStatement*: compareBlock,                        \
    retStatement*: compareReturn,                         \
    ifExpr*: compareIf,                                   \
    prefixExpr*: comparePrefix,                           \
    infixExpr*: compareInfix,                             \
    boolExpr*: compareBool,                               \
    integerLiteral*: compareInt,                          \
    invalidNode*: compareInvalid                          \
  )(left, right)

#define DescribeNode(node) _Generic((node),               \
    nodeWrapper*: describeNode,                           \
    rootNode*: describeRoot,                              \
    letStatement*: describeLet,                           \
    identifierNode*: describeId,                          \
    fnExpr*: describeFn,                                  \
    blockStatement*: describeBlock,                       \
    retStatement*: describeReturn,                        \
    ifExpr*: describeIf,                                  \
    prefixExpr*: describePrefix,                          \
    infixExpr*: describeInfix,                            \
    boolExpr*: describeBool,                              \
    integerLiteral*: describeInt,                         \
    invalidNode*: describeInvalid                         \
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

// rootNode
void* growRootNode (rootNode* rNode);
void pushNode (rootNode* rNode, nodeWrapper node);
nodeWrapper* getNode (rootNode* rNode, uint offset);
nodeWrapper* nextNode (rootNode* rNode);
nodeWrapper* iterator (rootNode* rNode);
void resetIterator (rootNode* rNode);


rootNode* mkRootNode ();
cstring rootAsString (rootNode* rNode);
nodeWrapper wrapRootNode (rootNode* node);
void unmkRootNode (rootNode** node);
bool compareRoot (rootNode* left, rootNode* right);
cstring describeRoot (rootNode* node);


// letStatement
letStatement* mkLetStatement (token id, nodeWrapper val);
cstring letAsString (letStatement* node);
nodeWrapper wrapLetNode (letStatement* node);
void unmkLetNode (letStatement** node);
bool compareLet (letStatement* left, letStatement* right);
cstring describeLet (letStatement* node);

// identifierNode
identifierNode* mkIdNode (token tkn);
cstring identifierAsString (identifierNode* node);
nodeWrapper wrapIdentifierNode (identifierNode* node);
void unmkIdNode (identifierNode** node);
bool compareId (
  identifierNode* left,
  identifierNode* right);
cstring describeId (identifierNode* node);


// fnExpr
fnExpr* mkFnNode (nodeWrapper params, nodeWrapper body);
cstring fnAsStr (fnExpr* fun);
nodeWrapper wrapFnNode (fnExpr* node);
void unmkFnNode (fnExpr** node);
bool compareFn (fnExpr* left, fnExpr* right);
cstring describeFn (fnExpr* node);


// blockStatement
blockStatement* mkBlockStatement (nodeList* statements);
cstring blockAsStr (blockStatement* block);
nodeWrapper wrapBlock (blockStatement* block);
void unmkBlockNode (blockStatement** node);
bool compareBlock (
  blockStatement* left,
  blockStatement* right);
cstring describeBlock (blockStatement* node);


// retStatement
retStatement* mkRetStatement (nodeWrapper val);
cstring returnAsString (retStatement* node);
nodeWrapper wrapReturnNode (retStatement* node);
void unmkReturnNode (retStatement** node);
bool compareReturn (
  retStatement* left,
  retStatement* right);
cstring describeReturn (retStatement* node);


// ifExpr
ifExpr* mkIfNode (
  token tkn,
  nodeWrapper cond,
  nodeWrapper body,
  nodeWrapper elseB);
cstring ifAsStr (ifExpr* node);
nodeWrapper wrapIfNode (ifExpr* node);
void unmkIfNode (ifExpr** node);
bool compareIf (ifExpr* left, ifExpr* right);
cstring describeIf (ifExpr* node);


// prefixExpr
prefixExpr* mkPrefixNode (
  token operator,
  nodeWrapper right);
cstring prefixAsStr (prefixExpr* node);
nodeWrapper wrapPrefixNode (prefixExpr* node);
void unmkPrefixNode (prefixExpr** node);
bool comparePrefix (prefixExpr* left, prefixExpr* right);
cstring describePrefix (prefixExpr* node);


// infixExpr
infixExpr* mkInfixNode (
  token operator,
  nodeWrapper left,
  nodeWrapper right);
cstring infixAsStr (infixExpr* node);
nodeWrapper wrapInfixNode (infixExpr* node);
void unmkInfixNode (infixExpr** node);
bool compareInfix (infixExpr* left, infixExpr* right);
cstring describeInfix (infixExpr* node);


// boolExpr
boolExpr* mkBoolNode (token tkn);
cstring boolAsStr (boolExpr* expr);
nodeWrapper wrapBoolNode (boolExpr* expr);
void unmkBoolNode (boolExpr** node);
bool compareBool (boolExpr* left, boolExpr* right);
cstring describeBool (boolExpr* node);


// integerLiteral
integerLiteral* mkIntNode (double number);
cstring intAsStr (integerLiteral* node);
nodeWrapper wrapIntNode (integerLiteral* node);
void unmkIntNode (integerLiteral** node);
bool compareInt (
  integerLiteral* left,
  integerLiteral* right);
cstring describeInt (integerLiteral* node);


// invalidNode
invalidNode* mkInvalidNode (token tkn);
cstring invalidAsString (invalidNode* node);
nodeWrapper wrapInvalidNode (invalidNode* node);
void unmkInvalidNode (invalidNode** node);
bool compareInvalid ();
cstring describeInvalid ();


// nodeWrapper
cstring wrapperAsString (nodeWrapper* node);
void unmkNodeWrapper (nodeWrapper* node);
bool compareNodeWrapper (
  nodeWrapper* left,
  nodeWrapper* right);
cstring describeNode (nodeWrapper* wrapper);
