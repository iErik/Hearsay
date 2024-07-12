#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "parser/nodes.h"
#include "utils/text.h"


rootNode* mkRootNode () {
  rootNode* root = make(rootNode);
  postMalloc(root);

  root->nodes = make(nodeWrapper);
  postMalloc(root->nodes);

  root->capacity = 10 * sizeof(nodeWrapper);
  root->length = 0;
  root->iteratorPos = 0;
  root->offsetPtr = root->nodes;

  return root;
}

void unmkRootNode (rootNode* rNode) {

}

void* growRootNode (rootNode* rNode) {
  if (rNode->capacity == 0) {
    rNode->nodes = (nodeWrapper*) calloc(10,
      sizeof(nodeWrapper));
    postMalloc(rNode->nodes);
    rNode->capacity = 10 * sizeof(nodeWrapper);

  } else {
    rNode->nodes = realloc(
      rNode->nodes,
      rNode->capacity * 2);
    postMalloc(rNode->nodes);
    rNode->capacity *= 2;
  }

  return rNode;
}

void pushNode (rootNode* rNode, nodeWrapper node) {
  if (rNode->capacity < rNode->capacity + sizeof(node)) {
    rNode = growRootNode(rNode);

    if (rNode->nodes == NULL) {
      printf(
        "ERROR - Could not push to nodeList: no memory");
      return;
    }
  }

  rNode->nodes[rNode->length] = node;
  rNode->offsetPtr = &rNode->nodes[rNode->length];
  rNode->length += 1;
}

nodeWrapper* nextNode (rootNode* rNode) {
  if (rNode->iteratorPos >= rNode->length)
    return NULL;

  nodeWrapper* node = &rNode->nodes[rNode->iteratorPos];
  rNode->iteratorPos++;

  return node;
}

nodeWrapper* iterator (rootNode* rNode) {
  return (rNode->nodes + rNode->iteratorPos);
}

void resetIterator (rootNode* rNode) {
  rNode->iteratorPos = 0;
}

nodeWrapper* getNode (rootNode* rNode, uint offset) {
  if ((offset + 1) > rNode->length) return NULL;

  return (rNode->nodes + offset);
}


letStatement* mkLetStatement (token letTkn, token idTkn) {
  if (letTkn.type != TknLet) return NULL;

  letStatement* node = make(letStatement);

  node->name = mkIdNode(idTkn);
  node->token = letTkn;

  return node;
}

retStatement* mkRetStatement (token retTkn) {
  if (retTkn.type != TknReturn) return NULL;

  retStatement* node = make(retStatement);

  node->token = retTkn;

  return node;
}

identifierNode* mkIdNode (token tkn) {
  if (tkn.type != TknIdent) return NULL;

  identifierNode* node = make(identifierNode);

  node->token = tkn;
  node->value = tkn.literal;

  return node;
}

void mkExpressionNode () {}

// -> AsString
// -------------------------

cstring rootAsString (rootNode* rNode) {
  cstring str;
  nodeWrapper* currNode = NULL;

  while ((currNode = nextNode(rNode)) != NULL)
    str = interpol("%s%s", str, wrapperAsString(currNode));

  return str;
}

cstring letAsString (letStatement* node) {
  if (node == NULL) return "";

  cstring literal = node->token.literal;

  cstring str = interpol("%s %s = %s;",
    literal,
    AsString(node->name),
    AsString(node->value));

  return str;
}

cstring returnAsString (retStatement* node) {
  if (node == NULL) return "";

  cstring literal = node->token.literal;

  cstring str = interpol("%s %s;",
    literal,
    AsString(node->value));

  return str;
}

cstring identifierAsString (identifierNode* node) {
  if (node == NULL) return "";

  return node->value;
}

cstring expressionAsString (expressionNode* node) {
  if (node == NULL) return "";

  return node->value;
}

cstring invalidAsString (invalidNode* node) {
  if (node == NULL) return "";

  return node->token.literal;
}

cstring wrapperAsString (nodeWrapper* wrapper) {
  if (wrapper == NULL) return "";

  switch (wrapper->type) {
    case RootNode:
      return rootAsString(wrapper->node);
    case IdentifierNode:
      return identifierAsString(wrapper->node);
    case ExpressionNode:
      return expressionAsString(wrapper->node);
    case LetStatement:
      return letAsString(wrapper->node);
    case ReturnStatement:
      return returnAsString(wrapper->node);
    case InvalidStatement:
    default:
      return "";
  }
}

// -> GetTokenLiteral
// ------------------------

cstring rootTokenLiteral (rootNode* rNode) {
  if (rNode->length == 0) return "";

  nodeWrapper* firstNode = getNode(rNode, 0);
  return getTokenLiteral(firstNode);
}

cstring getTokenLiteral (nodeWrapper* wrapper) {
  void* node = wrapper->node;

  switch (wrapper->type) {
    case RootNode:
      return rootTokenLiteral(wrapper->node);
    case IdentifierNode:
      return ((identifierNode *) node)->token.literal;
    case ExpressionNode:
      return ((expressionNode *) node)->token.literal;
    case LetStatement:
      return ((letStatement *) node)->token.literal;
    case ReturnStatement:
      return ((retStatement *) node)->token.literal;
    case InvalidStatement:
    default:
      return "";
  }
}

// -> Wrappers
// -----------

nodeWrapper wrapRootNode (rootNode* node) {
  return (nodeWrapper) {
    .type = RootNode,
    .node = node
  };
}

nodeWrapper wrapIdentifierNode (identifierNode* node) {
  return (nodeWrapper) {
    .type = IdentifierNode,
    .node = node
  };
}

nodeWrapper wrapExpressionNode (expressionNode* node) {
  return (nodeWrapper) {
    .type = ExpressionNode,
    .node = node
  };
}

nodeWrapper wrapLetNode (letStatement* node) {
  return (nodeWrapper) {
    .type = LetStatement,
    .node = node
  };
}

nodeWrapper wrapReturnNode (retStatement* node) {
  return (nodeWrapper) {
    .type = ReturnStatement,
    .node = node
  };
}

nodeWrapper wrapInvalidNode (invalidNode* node) {
  return (nodeWrapper) {
    .type = InvalidStatement,
    .node = node
  };
}
