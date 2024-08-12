#include <stdint.h>

#include "tests/test-utils.h"
#include "parser/nodes.h"
#include "utils/text.h"


#define INITIAL_ROOT_CAPACITY (10 * sizeof(nodeWrapper))


void testGrowList () {
  testing("nl_grow");

  nodeList* list = nl_new();

  expect(list->nodes != NULL,
    "rootNode->nodes must not be a NULL pointer\n"
    "Received: %p\n", list->nodes);
  expect(list->capacity == INITIAL_ROOT_CAPACITY,
    "rootNode.capacity must be 10 * sizeof(nodeWrapper)\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    INITIAL_ROOT_CAPACITY, list->capacity);

  nl_grow(list);

  expect(list->nodes != NULL,
    "rootNode->nodes must not be a NULL pointer\n"
    "Received: %p\n", list->nodes);
  expect(list->capacity == INITIAL_ROOT_CAPACITY * 2,
    "rootNode.capacity must be "
    "(10 * sizeof(nodeWrapper)) * 2\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    INITIAL_ROOT_CAPACITY * 2, list->capacity);

  nl_grow(list);

  expect(list->nodes != NULL,
    "rootNode->nodes must not be a NULL pointer\n"
    "Received: %p\n", list->nodes);
  expect(list->capacity == (INITIAL_ROOT_CAPACITY * 2) * 2,
    "rootNode.capacity must be "
    "((10 * sizeof(nodeWrapper)) * 2) * 2\n"
    "Expected: %lu\n"
    "Received: %lu\n",
    (INITIAL_ROOT_CAPACITY * 2) * 2, list->capacity);

  if (list != NULL) free(list);
}

void testPushNode () {
  testing("nl_push");

  nodeList* list = nl_new();
  nodeWrapper nodeOne = INVALID_STATEMENT;

  nl_push(list, nodeOne);
  expectEq(list->length, 1,
    "List length must be 1");

  expect((list->nodes + 1) != NULL,
    "First childNode should not be a NULL pointer\n"
    "Recevied: %p\n", (list->nodes + 1));

  expect(nl_get(list, 0)->type == InvalidStatement,
    "First childNode has to be of type %i\n"
    "Received: %i\n",
    InvalidStatement, nl_get(list, 0)->type);

  for (int i = 0; i < 10; i++)
    nl_push(list, nodeOne);

  expectEq(list->length, 11,
    "List length must be 11");

  expectEq(list->capacity, NODE_LIST_INIT_CAP * 2,
    "List's capacity must have doubled");

  nodeWrapper nodeTwo = { .type = LetStatement };
  nl_push(list, nodeTwo);
  expect(list->offsetPtr != NULL,
    "rootNode.offsetPtr must not be a NULL ptr\n"
    "Received: %p\n", list->offsetPtr);
  expect(list->offsetPtr->type == LetStatement,
    "rootNode.offsetPtr->type must be %i\n"
    "Expected: %i\n"
    "Received: %i\n",
    LetStatement, LetStatement, list->offsetPtr->type);
  expect(nl_get(list, 11)->type == LetStatement,
    "node at index %i should be of type %i\n"
    "Expected: %i\n"
    "Received: %i\n",
    11, LetStatement, LetStatement,
    nl_get(list, 11)->type);


  if (list != NULL)
    free(list);
}

void testIterator () {
  testing("rootIterator");

  nodeList* list = nl_new();

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
    nl_push(list, (nodeWrapper) { .type = nodeTypes[i] });

  expectEq(list->length, 10,
    "List must have 10 child nodes");

  nodeWrapper* currentItem = NULL;
  nodeWrapper* lastValue = NULL;

  expectEq(list->iteratorPos, 0,
    "List iterator position must be 0");

  expectEq(
    (uintptr_t) list->offsetPtr,
    (uintptr_t) nl_get(list, 9),
    "List.offsetPtr must point to the last item");

  while ((currentItem = nl_nextNode(list)) != NULL) {
    expect(
      currentItem->type == nodeTypes[list->iteratorPos - 1],
      "current iteration's node type must match "
      "the respective type in nodeTypes list\n"
      "Expected: %i\n"
      "Received: %i\n"
      "rootNode->iteratorPos: %lu\n",
      nodeTypes[list->iteratorPos],
      currentItem->type,
      list->iteratorPos);

    lastValue = currentItem;
  }

  expectEq(list->iteratorPos, 10,
    "List's iterator position must be 10");

  expect(currentItem == NULL,
    "current iteration item must be a NULL pointer\n"
    "Expected: %p\n"
    "Received: %p\n",
    NULL, currentItem);

  expect(lastValue == list->offsetPtr,
    "Last iteration value must be equal to "
    "root->offsetPtr\n"
    "Expected: %p\n"
    "Received: %p\n",
    list->offsetPtr, lastValue);

  nl_resetIterator(list);

  expect(nl_iterator(list) == nl_get(list, 0),
    "iterator must be pointing to the first node again\n"
    "Expected: %p\n"
    "Received: %p\n",
    nl_get(list, 0), nl_iterator(list));

  expectEq(list->iteratorPos, 0,
    "List iterator's position must be 0 again");

  if (list != NULL)
    free(list);
}

void TestNodeList () {
  testing("nodeList");

  nodeList* list = nl_new();

  expect(list != NULL,
    "rootNode must not be a NULL pointer\n"
    "Received: %p\n", list);

  expectEq(list->length, 0, "List length (0)");

  expectEq(list->capacity, NODE_LIST_INIT_CAP,
    interpol("List capacity (%d)", NODE_LIST_INIT_CAP));

  testGrowList();
  testPushNode();
  testIterator();

  if (list != NULL) free(list);
}


