#pragma once

#include <stddef.h>
#include "lexer/tokens.h"

// ROOT:
//  - Statements []Statements{}

// Interfaces:
//  - Node { TokenLiteral() string, asString() string }
//  - Statement { Node + statementNode() }
//  - Expression { Node + expressionNode() }


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
}


// ----------------------------------------------------- //
// -> Enums / Constants                                  //
// ----------------------------------------------------- //

typedef enum {
  RootNode,

  IdentifierNode,
  ExpressionNode,

  IfExpression,
  FunctionLiteral,
  IntegerLiteral,
  InfixExpression,
  PrefixExpression,
  BooleanExpression,

  LetStatement,
  ReturnStatement,
  InvalidStatement
} nodeType;


// ----------------------------------------------------- //
// -> NodeList (Temporary)                               //
// ----------------------------------------------------- //

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
  nodeType type;
  void*    node;
} nodeWrapper;

typedef nodeWrapper exprWrapper;


typedef struct rootNode {
  nodeWrapper* nodes;
  nodeWrapper* offsetPtr;

  size_t       length;
  size_t       capacity;
  size_t       iteratorPos;
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
  exprWrapper expression;
} exprStatement;

typedef struct letStatement {
  token          token;
  identifierNode name;
  exprWrapper    value;
} letStatement;

typedef struct retStatement {
  token        token;
  exprWrapper* value;
} retStatement;

typedef struct prefixExpr {
  token       token;
  cstring     operator;
  exprWrapper right;
} prefixExpr;

typedef struct infixExpr {
  token       token;
  exprWrapper left;
  cstring     operator;
  exprWrapper right;
} infixExpr;

typedef struct ifExpr {
  token          token;
  cstring        condition;
  blockStatement body;
  blockStatement elseBlock;
} ifExpr;

typedef struct boolExpr {
  token token;
} boolExpr;

typedef struct fnExpr {
  token           token;
  identifierNode* params;
  blockStatement  body;
} fnExpr;

typedef struct integerLiteral {
  token token;
  int   value;
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


letStatement* mkLetStatement (token letTkn, token idTkn);
retStatement* mkRetStatement (token retTkn);
identifierNode* mkIdNode (token tkn);
//expressionNode* mkExpressionNode ();
invalidNode* mkInvalidNode (token tkn);


cstring rootAsString (rootNode* rNode);
cstring letAsString (letStatement* node);
cstring returnAsString (retStatement* node);
cstring identifierAsString (identifierNode* node);
//cstring expressionAsString (expressionNode* node);
cstring invalidAsString (invalidNode* node);
cstring wrapperAsString (nodeWrapper* node);


cstring rootTokenLiteral (rootNode* rNode);
cstring getTokenLiteral (nodeWrapper* wrapper);


nodeWrapper wrapRootNode (rootNode* node);
nodeWrapper wrapIdentifierNode (identifierNode* node);
//nodeWrapper wrapExpressionNode (expressionNode* node);
exprWrapper wrapExprNode (void* node);
nodeWrapper wrapLetNode (letStatement* node);
nodeWrapper wrapReturnNode (retStatement* node);
nodeWrapper wrapInvalidNode (invalidNode* node);

