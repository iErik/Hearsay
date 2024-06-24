#include "tests/test-utils.h"
#include "parser/nodes.h"

statementNode mkTestNode (statementType type) {
  statementNode node = {
    .type = type
  };

  return node;
}

void TestNodeList () {
  nodeList* list = mkNodeList();

  expect(list->capacity > 0, "List capacity mismatch\n"
    "Expected: %i\n"
    "Received: %i\n",
    10, list->capacity);

  expect(list->length == 0, "List length mismatch\n"
    "Expected: %i\n"
    "Received: %i\n",
    0, list->length);

  statementNode testNode = mkTestNode(LetStatement);
  pushNode(list, testNode);

  expect(list->length == 1, "List length mismatch\n"
    "Expected: %i\n"
    "Received: %i\n",
    1, list->length);

  expect(list->nodes[0].type == testNode.type,
    "Node mismatch\n"
    "Expected: %s\n"
    "Received: %s\n",
    testNode.type, list->nodes[0].type);

  statementType previousType = testNode.type;
  testNode = mkTestNode(ReturnStatement);
  pushNode(list, testNode);

  expect(list->length == 2, "List length mismatch\n"
    "Expected: %i\n"
    "Received: %i\n",
    1, list->length);

  expect(list->nodes[0].type == previousType,
    "Node mismatch\n"
    "Expected: %s\n"
    "Received: %s\n",
    previousType, list->nodes[0].type);

  expect(list->nodes[1].type == testNode.type,
    "Node mismatch\n"
    "Expected: %s\n"
    "Received: %s\n",
    testNode.type, list->nodes[1].type);
}

int main () {
  TestNodeList();

  return 0;
}
