#include <stdlib.h>

#include "parser/nodes.h"

nodeList* mkNodeList () {
  nodeList* list = (nodeList *) malloc(sizeof(nodeList));
  postMalloc(list);

  list->nodes = (statementNode *) calloc(10,
    sizeof(statementNode));
  list->capacity = 10 * sizeof(statementNode);
  list->length = 0;

  return list;
}

void* growNodeList (nodeList* list) {
  if (list->capacity == 0) {
    list->nodes = (statementNode *) calloc(10,
      sizeof(statementNode));
    postMalloc(list->nodes);
    list->capacity = sizeof(statementNode);
  } else {
    list->nodes = realloc(list->nodes, list->capacity * 2);
    postMalloc(list->nodes);
    list->capacity += 2;
  }

  return list;
}

void pushNode (nodeList* list, statementNode node) {
  if (list->capacity < list->capacity + sizeof(node)) {
    list = growNodeList(list);

    if (list == NULL) {
      printf(
          "ERROR - Could not push to nodeList: no memory");
      return;
    }
  }

  list->nodes[list->length] = node;
  list->length += 1;
}


programNode* mkProgramNode () {
  return NULL;
}

statementNode mkLetStatement (letStatement* fields) {
  return (statementNode) {
    .type = LetStatement,
    .fields = (void *) fields,
  };
}

identifierNode mkIdNode (token tkn, cstring value) {
  return (identifierNode) { .token = tkn, .value = value };
}

void mkExpressionNode () {}


cstring identTokenLiteral (identifierNode* node) {
  return node->token.literal;
}

cstring letTokenLiteral (letStatement* node) {
  return node->token.literal;
}

cstring programTokenLiteral (programNode* node) {
  if (node->statements->length == 0) return "";

  return "";
  //return getNode(node->stateeges, 0)
}

