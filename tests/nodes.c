#include <string.h>

#include "common.h"
#include "test-utils.h"
#include "parser/nodes.h"

statementNode mkTestNode (cstring type) {
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

  statementNode testNode = mkTestNode("TEST 1");
  pushNode(list, testNode);

  expect(list->length == 1, "List length mismatch\n"
    "Expected: %i\n"
    "Received: %i\n",
    1, list->length);

  expect(strcmp(list->nodes[0].type, testNode.type) == 0,
    "Node mismatch\n"
    "Expected: %s\n"
    "Received: %s\n",
    testNode.type, list->nodes[0].type);

  cstring previousType = testNode.type;
  testNode = mkTestNode("TEST 2");
  pushNode(list, testNode);

  expect(list->length == 2, "List length mismatch\n"
    "Expected: %i\n"
    "Received: %i\n",
    1, list->length);

  expect(strcmp(list->nodes[0].type, previousType) == 0,
    "Node mismatch\n"
    "Expected: %s\n"
    "Received: %s\n",
    previousType, list->nodes[0].type);

  expect(strcmp(list->nodes[1].type, testNode.type) == 0,
    "Node mismatch\n"
    "Expected: %s\n"
    "Received: %s\n",
    testNode.type, list->nodes[1].type);
}

int main () {
  TestNodeList();

  return 0;
}
