#include "tests/test-utils.h"
#include "utils/text.h"
#include "parser/nodes.h"

#define INITIAL_ROOT_CAPACITY (10 * sizeof(nodeWrapper))

void testGrowRootNode () {
  testing("growRootNode");

  rootNode* root = mkRootNode();

  expect(root->nodes != NULL,
    "rootNode->nodes must not be a NULL pointer\n"
    "Received: %p\n", root->nodes);
  expect(root->capacity == INITIAL_ROOT_CAPACITY,
    "rootNode.capacity must be 10 * sizeof(nodeWrapper)\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    INITIAL_ROOT_CAPACITY, root->capacity);

  growRootNode(root);

  expect(root->nodes != NULL,
    "rootNode->nodes must not be a NULL pointer\n"
    "Received: %p\n", root->nodes);
  expect(root->capacity == INITIAL_ROOT_CAPACITY * 2,
    "rootNode.capacity must be "
    "(10 * sizeof(nodeWrapper)) * 2\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    INITIAL_ROOT_CAPACITY * 2, root->capacity);

  growRootNode(root);

  expect(root->nodes != NULL,
    "rootNode->nodes must not be a NULL pointer\n"
    "Received: %p\n", root->nodes);
  expect(root->capacity == (INITIAL_ROOT_CAPACITY * 2) * 2,
    "rootNode.capacity must be "
    "((10 * sizeof(nodeWrapper)) * 2) * 2\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    (INITIAL_ROOT_CAPACITY * 2) * 2, root->capacity);

  if (root != NULL) free(root);
}

void testPushNode () {
  testing("pushNode");

  rootNode* root = mkRootNode();
  nodeWrapper nodeOne = INVALID_STATEMENT;

  pushNode(root, nodeOne);
  expect(root->length == 1,
    "rootNode.length must be 1\n"
    "Expected: %i\n"
    "Received: %i\n",
    1, root->length);

  expect((root->nodes + 1) != NULL,
    "First childNode should not be a NULL pointer\n"
    "Recevied: %p\n", (root->nodes + 1));
  expect(getNode(root, 0)->type == InvalidStatement,
    "First childNode has to be of type %i\n"
    "Received: %i\n",
    InvalidStatement, getNode(root, 0)->type);

  for (int i = 0; i < 10; i++)
    pushNode(root, nodeOne);

  expect(root->length == 11,
    "rootNode.length must be 11\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    11, root->length);
  expect(root->capacity = INITIAL_ROOT_CAPACITY * 2,
    "rootNode.capacity must have doubled\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    INITIAL_ROOT_CAPACITY * 2, root->capacity);

  nodeWrapper nodeTwo = { .type = LetStatement };
  pushNode(root, nodeTwo);
  expect(root->offsetPtr != NULL,
    "rootNode.offsetPtr must not be a NULL ptr\n"
    "Received: %p\n", root->offsetPtr);
  expect(root->offsetPtr->type == LetStatement,
    "rootNode.offsetPtr->type must be %i\n"
    "Expected: %i\n"
    "Received: %i\n",
    LetStatement, LetStatement, root->offsetPtr->type);
  expect(getNode(root, 11)->type == LetStatement,
    "node at index %i should be of type %i\n"
    "Expected: %i\n"
    "Received: %i\n",
    11, LetStatement, LetStatement,
    getNode(root, 11)->type);


  if (root != NULL)
    free(root);
}

void testRootIterator () {
  testing("rootIterator");

  rootNode* root = mkRootNode();
  nodeType nodeTypes[10] = {
    RootNode,
    LetStatement,
    InvalidStatement,
    LetStatement,
    IdentifierNode,
    ExpressionNode,
    ReturnStatement,
    RootNode,
    InvalidStatement,
    ExpressionNode,
  };

  for (int i = 0; i < 10; i++)
    pushNode(root, (nodeWrapper) { .type = nodeTypes[i] });

  expect(root->length == 10,
    "rootNode must have 10 child nodes\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    10, root->length);

  nodeWrapper* currentItem = NULL;
  nodeWrapper* lastValue = NULL;

  expect(root->iteratorPos == 0,
    "rootNode.iteratorPos must be 0\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    0, root->iteratorPos);

  expect(root->offsetPtr == getNode(root, 9),
    "rootNode.offsetPtr must point to last item\n"
    "Expected: %p\n"
    "Received: %p\n",
    root->offsetPtr, getNode(root, 9));

  while ((currentItem = nextNode(root)) != NULL) {
    expect(
      currentItem->type == nodeTypes[root->iteratorPos - 1],
      "current iteration's node type must match "
      "the respective type in nodeTypes list\n"
      "Expected: %i\n"
      "Received: %i\n"
      "rootNode->iteratorPos: %lu\n",
      nodeTypes[root->iteratorPos],
      currentItem->type,
      root->iteratorPos);

    lastValue = currentItem;
  }

  expect(root->iteratorPos == 10,
    "rootNode.iteratorPos must be 10\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    10, root->iteratorPos);

  expect(currentItem == NULL,
    "current iteration item must be a NULL pointer\n"
    "Expected: %p\n"
    "Received: %p\n",
    NULL, currentItem);

  expect(lastValue == root->offsetPtr,
    "Last iteration value must be equal to "
    "root->offsetPtr\n"
    "Expected: %p\n"
    "Received: %p\n",
    root->offsetPtr, lastValue);

  resetIterator(root);

  expect(iterator(root) == getNode(root, 0),
    "iterator must be pointing to the first node again\n"
    "Expected: %p\n"
    "Received: %p\n",
    getNode(root, 0), iterator(root));
  expect(root->iteratorPos == 0,
    "root->iteratorPos be 0 again.\n"
    "Expected: %d\n"
    "Received: %d\n",
    0, root->iteratorPos);

  if (root != NULL)
    free(root);
}

void TestRootNode () {
  testing("rootNode");

  rootNode* root = mkRootNode();

  expect(root != NULL,
    "rootNode must not be a NULL pointer\n"
    "Received: %p\n", root);

  expect(root->length == 0,
    "rootNode.length has to be 0\n"
    "Expected: %i\n"
    "Received: %i\n",
    0, root->length);

  expect(root->capacity == INITIAL_ROOT_CAPACITY,
    "rootNode.capacity has to be %lu\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    INITIAL_ROOT_CAPACITY,
    INITIAL_ROOT_CAPACITY,
    root->capacity);

  testGrowRootNode();
  testPushNode();
  testRootIterator();

  if (root != NULL) free(root);
}

void TestLetStatement () {
  testing("LetStatement");

  token letTkn = { .literal = "let", .type = TknLet };
  token idTkn  = { .literal = "test", .type = TknIdent };

  letStatement* letNode = mkLetStatement(letTkn, idTkn);

  expect(letNode != NULL,
    "LetStatement node must not be NULL\n"
    "Received: %p\n", letNode);

  expect(letNode->name != NULL,
    "LetStatement node's name must not be NULL\n"
    "Received: %p\n", letNode->name);
  expect(letNode->name->token.type == idTkn.type,
    "LetStatement's name token type must be TknIdent\n"
    "Expected: %i\n"
    "Received: %i\n",
    idTkn.type, letNode->name->token.type);
  expect(streq(
    letNode->name->token.literal, idTkn.literal),
    "LetStatement's name token literal must match\n"
    "Expected: %s\n"
    "Received: %s\n",
    idTkn.literal, letNode->name->token.literal);
  expect(streq(
    letNode->name->value, idTkn.literal),
    "LetStatement's name value must match\n"
    "Expected: %s\n"
    "Received: %s\n",
    idTkn.literal, letNode->name->value);

  expect(letNode->token.type == letTkn.type,
    "LetStatements token's type must be TknLet\n"
    "Expected: %i\n"
    "Received: %i\n",
    letTkn.type, letNode->token.type);
  expect(streq(letNode->token.literal, letTkn.literal),
    "LetStatement's token.literal must be 'let'\n"
    "Expected: %s\n"
    "Received: %s\n",
    letTkn.literal, letNode->token.literal);


  if (letNode != NULL) free(letNode);
}

void TestReturnStatement () {
  testing("ReturnStatement");

  token retTkn  = { .literal = "ret", .type = TknReturn };

  retStatement* retNode = mkRetStatement(retTkn);

  expect(retNode != NULL,
    "ReturnStatement node must not be NULL\n"
    "Received: %p\n", retNode);

  expect(retNode->token.type == retTkn.type,
    "ReturnStatement's token's type must be TknReturn\n"
    "Expected: %i\n"
    "Received: %i\n",
    retTkn.type, retNode->token.type);
  expect(streq(retNode->token.literal, retTkn.literal),
    "ReturnStatement's token.literal must be 'ret'\n"
    "Expected: %s\n"
    "Received: %s\n",
    retTkn.literal, retNode->token.literal);


  if (retNode != NULL) free(retNode);
}

void TestIdentifierNode () {
  testing("IdentifierNode");

  token idTkn  = { .literal = "TestId", .type = TknIdent };

  identifierNode* idNode = mkIdNode(idTkn);

  expect(idNode != NULL,
    "IdentifierNode node must not be NULL\n"
    "Received: %p\n", idNode);

  expect(idNode->token.type == idTkn.type,
    "IdentifierNode's token's type must be TknIdent\n"
    "Expected: %i\n"
    "Received: %i\n",
    idTkn.type, idNode->token.type);
  expect(streq(idNode->token.literal, idTkn.literal),
    "IdentifierNode's token.literal must match\n"
    "Expected: %s\n"
    "Received: %s\n",
    idTkn.literal, idNode->token.literal);
  expect(streq(idNode->value, idTkn.literal),
    "IdentifierNode's value must match token literal\n"
    "Expected: %s\n"
    "Received: %s\n",
    idTkn.literal, idNode->value);


  if (idNode != NULL) free(idNode);
}

void TestAsString () {
  cstring letStr = "let variable = 4;";
  //cstring retStr = "ret variable;";
  //cstring rootStr = "let variable = 4; ret variable;";

  letStatement* letNode = mkLetStatement(
    (token) { .type = TknLet, .literal = "let" },
    (token) { .type = TknIdent, .literal = "variable" }
  );

  /*
  retStatement* retNode = mkRetStatement(
    (token) { .type = TknReturn, .literal = "ret" }
  );
  */

  rootNode* root = mkRootNode();
  pushNode(root, WrapNode(letNode));

  expect(streq(AsString(letNode), letStr),
    "LetStatement.AsString must match input.\n"
    "Expected: %s\n"
    "Received: %s\n",
    letStr, AsString(letNode));

  expect(streq(AsString(root), letStr),
    "RootNode.AsString must match input.\n"
    "Expected: %s\n"
    "Received: %s\n",
    letStr, AsString(root));

  if (root != NULL) free(root);
  if (letNode != NULL) free(letNode);
}

void TestTokenLiteral () {}

void TestNodeWrapper () {

}

int main () {
  TestRootNode();
  TestLetStatement();
  TestReturnStatement();
  TestIdentifierNode();
  TestAsString();
  TestTokenLiteral();
  TestNodeWrapper();

  return 0;
}
