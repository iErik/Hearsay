#pragma once

#include <stddef.h>
#include "lexer/tokens.h"

// ROOT:
//  - Statements []Statements{}

// Interfaces:
//  - Node { TokenLiteral() string, asString() string }
//  - Statement { Node + statementNode() }
//  - Expression { Node + expressionNode() }


// -> Macros
// ---------

#define AsString(node) _Generic((node),                   \
    identifierNode*: identifierAsString,                  \
    expressionNode*: expressionAsString,                  \
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
    expressionNode*: wrapExpressionNode,                  \
    letStatement*: wrapLetNode,                           \
    retStatement*: wrapReturnNode,                        \
    invalidNode*: wrapInvalidNode                         \
  )(node)

#define INVALID_STATEMENT (nodeWrapper) {                 \
  .type = InvalidStatement                                \
}

// -> Constants
// ------------

typedef enum {
  RootNode,

  IdentifierNode,
  ExpressionNode,

  LetStatement,
  ReturnStatement,
  InvalidStatement
} nodeType;

// -> Types
// --------

typedef struct nodeWrapper {
  nodeType type;
  void* node;
} nodeWrapper;

typedef struct rootNode {
  nodeWrapper* nodes;
  nodeWrapper* offsetPtr;

  size_t capacity;
  size_t length;
  size_t iteratorPos;
} rootNode;

typedef struct identifierNode {
  token token;
  cstring value;
} identifierNode;

typedef struct expressionNode {
  token token;
  cstring value;
} expressionNode;

typedef struct letStatement {
  token token;
  identifierNode* name;
  expressionNode* value;
} letStatement;

typedef struct retStatement {
  token token;
  expressionNode* value;
} retStatement;

typedef struct invalidNode {
  token token;
} invalidNode;

// -> Functions
// ------------

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


cstring rootAsString (rootNode* rNode);
cstring letAsString (letStatement* node);
cstring returnAsString (retStatement* node);
cstring identifierAsString (identifierNode* node);
cstring expressionAsString (expressionNode* node);
cstring invalidAsString (invalidNode* node);
cstring wrapperAsString (nodeWrapper* node);


cstring rootTokenLiteral (rootNode* rNode);
cstring getTokenLiteral (nodeWrapper* wrapper);


nodeWrapper wrapRootNode (rootNode* node);
nodeWrapper wrapIdentifierNode (identifierNode* node);
nodeWrapper wrapExpressionNode (expressionNode* node);
nodeWrapper wrapLetNode (letStatement* node);
nodeWrapper wrapReturnNode (retStatement* node);
nodeWrapper wrapInvalidNode (invalidNode* node);

