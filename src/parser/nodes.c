#include <stdlib.h>

#include "parser/nodes.h"
#include "utils/text.h"


parserNode* mkRootNode () {
  rootNode* root = make(rootNode);
  postMalloc(root);

  root->nodes = make(parserNode);
  postMalloc(root->nodes);

  root->capacity = 10 * sizeof(parserNode);
  root->length = 0;
  root->iteratorPtr = root->nodes;
  root->offsetPtr = root->nodes;

  parserNode* node = make(parserNode);
  postMalloc(node);
  node->type = RootNode;
  node->meta = root;

  return node;
}

void* growRootNode (parserNode* node) {
  rootNode* root = node->meta;

  if (root->capacity == 0) {
    root->nodes = (parserNode*) calloc(10,
      sizeof(parserNode));
    postMalloc(root->nodes);
    root->capacity = 10 * sizeof(parserNode);

  } else {
    root->nodes = realloc(root->nodes, root->capacity * 2);
    postMalloc(root->nodes);
    root->capacity *= 2;
  }

  return node;
}

void pushNode (parserNode* rNode, parserNode node) {
  rootNode* root = (rootNode *) rNode->meta;

  if (root->capacity < root->capacity + sizeof(node)) {
    root = growRootNode(rNode);

    if (root == NULL) {
      printf(
        "ERROR - Could not push to nodeList: no memory");
      return;
    }
  }

  root->nodes[root->length] = node;
  root->length += 1;
}

void* iterateNodes (parserNode* rNode) {
  rootNode* root = (rootNode *) rNode->meta;

  if (root->iteratorPtr == NULL)
    return NULL;

  parserNode* currValue = root->iteratorPtr;

  if (root->iteratorPtr == root->offsetPtr)
    root->iteratorPtr = NULL;
  else
    root->iteratorPtr++;

  return currValue;
}

void resetIterator (parserNode* rNode) {
  rootNode* root = (rootNode *) rNode->meta;
  root->iteratorPtr = root->nodes;
}

parserNode* getNode (parserNode* rNode, uint offset) {
  rootNode* root = (rootNode*) rNode->meta;
  return (root->nodes + offset);
}


parserNode mkLetStatement (token letTkn, token idTkn) {
  letStatement* meta = make(letStatement);

  meta->name = mkIdNode(idTkn);

  parserNode node = {
    .type  = LetStatement,
    .token = letTkn,
    .meta  = meta
  };

  return node;
}

parserNode mkRetStatement (token retTkn) {
  retStatement* meta = make(retStatement);

  parserNode node = {
    .type  = ReturnStatement,
    .token = retTkn,
    .meta  = meta
  };

  return node;
}

parserNode mkIdNode (token tkn) {
  identifierNode* meta = make(identifierNode);

  meta->value = tkn.literal;

  parserNode node = {
    .type = IdentifierNode,
    .token = tkn,
    .meta = meta
  };

  return node;
}

void mkExpressionNode () {}

// -> AsString
// -------------------------

cstring rootAsString (parserNode* rNode) {
  cstring str;
  parserNode* currNode;

  while ((currNode = iterateNodes(rNode)) != NULL)
    str = interpol("%s%s", str, nodeAsString(currNode));

  return str;
}

cstring letAsString (parserNode* node) {
  letStatement* letStat = node->meta;
  cstring literal = node->token.literal;

  cstring str = interpol("%s %s = %s;",
    literal,
    nodeAsString(&letStat->name),
    nodeAsString(&letStat->value));

  return str;
}

cstring returnAsString (parserNode* node) {
  retStatement* retStat = node->meta;
  cstring literal = node->token.literal;

  cstring str = interpol("%s %s;",
    literal,
    nodeAsString(&retStat->value));

  return str;
}

cstring identifierAsString (parserNode* node) {
  identifierNode* id = node->meta;
  return id->value;
}

cstring expressionAsString (parserNode* node) {
  expressionNode* expr = node->meta;
  return expr->value;
}

// -> GetTokenLiteral
// ------------------------

cstring rootTokenLiteral (parserNode* node) {
  rootNode* root = node->meta;

  if (root->length == 0) return "";

  parserNode* firstNode = getNode(node, 0);
  return getTokenLiteral(firstNode);
}

cstring getTokenLiteral (parserNode* node) {
  switch (node->type) {
    case RootNode:
      return rootTokenLiteral(node);

    case IdentifierNode:
    case ExpressionNode:

    case LetStatement:
    case ReturnStatement:
    case InvalidStatement:
    default:
      return node->token.literal;
  }
}

cstring nodeAsString (parserNode* node) {
  switch (node->type) {
    case RootNode:
      return rootAsString(node);
    case IdentifierNode:
      return identifierAsString(node);
    case ExpressionNode:
      return expressionAsString(node);
    case LetStatement:
      return letAsString(node);
    case ReturnStatement:
      return returnAsString(node);
    case InvalidStatement:
      return node->token.literal;
    default:
      return "";
  }
}
