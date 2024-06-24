#pragma once

#include <stddef.h>
#include "lexer/tokens.h"

// -> Macros
// ---------

/*
#define TokenLiteral(node) \
  node->vtable->getTokenLiteral(node)
*/

#define TokenLiteral(node) _Generic(typeof(node) \
  )

#define StatementNode(node) \
  node->vtable->getStatementNode(node)

#define ExpressionNode(node) \
  node->vtable->getExpressionNode(node)

#define AsString (node) \
  nodes->vtable->asString(node)

#define INVALID_STATEMENT (parserNode) { \
  .type = InvalidStatement                  \
}

// ROOT:
//  - Statements []Statements{}

// Interfaces:
//  - Node { TokenLiteral() string, asString() string }
//  - Statement { Node + statementNode() }
//  - Expression { Node + expressionNode() }

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

typedef struct parserNode {
  nodeType type;
  token token;

  void* meta;
} parserNode;

typedef struct rootNode {
  parserNode* nodes;
  parserNode* offsetPtr;
  parserNode* iteratorPtr;

  size_t capacity;
  size_t length;
} rootNode;

typedef struct identifierNode {
  cstring value;
} identifierNode;

typedef struct expressionNode {
  cstring value;
} expressionNode;

typedef struct letStatement {
  // Identifier
  parserNode name;
  // Expression
  parserNode value;
} letStatement;

typedef struct retStatement {
  // Expression
  parserNode value;
} retStatement;


// -> Functions
// ------------

parserNode* mkRootNode ();

void* growRootNode (parserNode* node);

void pushNode (parserNode* rootNode, parserNode node);

void* iterateNodes (parserNode* rootNode);

parserNode* getNode(parserNode* rootNode, uint offset);


parserNode* mkProgramNode ();

parserNode mkLetStatement (token letTkn, token idTkn);

parserNode mkRetStatement (token retTkn);

parserNode mkIdNode (token tkn);


cstring getTokenLiteral (parserNode* node);

cstring nodeAsString (parserNode* node);
