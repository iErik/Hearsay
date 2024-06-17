#pragma once

#include <stddef.h>
#include "tokens.h"

// -> Macros
// ---------

#define TokenLiteral(node) \
  node->vtable->getTokenLiteral(node)

#define StatementNode(node) \
  node->vtable->getStatementNode(node)

#define ExpressionNode(node) \
  node->vtable->getExpressionNode(node)

#define LET_NODE {         \
  .type   = "LET",         \
}

#define INVALID_STATEMENT (statementNode) { \
  .type = InvalidStatement                  \
}

// ROOT:
//  - Statements []Statements{}

// -> Constants
// ------------

typedef enum {
  LetStatement,
  InvalidStatement
} statementType;

// -> Types
// --------

typedef struct statementNode {
  statementType type;
  void*         fields;
  void*         vtable;
} statementNode;

typedef struct programNode {
  struct nodeList* statements;
} programNode;

typedef struct identifierNode {
  token   token;
  cstring value;
} identifierNode;

typedef struct expressionNode {

} expressionNode;

typedef struct letStatement {
  token           token;
  identifierNode  name;
  expressionNode  value;
} letStatement;

typedef struct nodeVTable {
  cstring         (*getTokenLiteral)   ();
  void*           (*getStatementNode)  ();
  expressionNode* (*getExpressionNode) ();
} nodeVTable;

typedef struct nodeList {
  statementNode* nodes;
  size_t         capacity;
  size_t         length;
} nodeList;


// -> Functions
// ------------

nodeList* mkNodeList ();

void* growNodeList (nodeList* list);

void pushNode (nodeList* list, statementNode node);


programNode* mkProgramNode ();

statementNode mkLetStatement (token tkn);

identifierNode mkIdNode (token tkn, cstring value);

