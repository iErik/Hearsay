#include "common.h"
#include "parser/nodes.h"

nodeList* nl_new () {
  nodeList* root = make(nodeList);
  postMalloc(root);

  root->nodes = cmaken(NODE_LIST_INIT_CAP, nodeWrapper);

  postMalloc(root->nodes);

  root->capacity = NODE_LIST_INIT_CAP;
  root->length = 0;
  root->iteratorPos = 0;
  root->offsetPtr = root->nodes;

  return root;
}

void nl_destroy (nodeList** nList) { }

void* nl_grow (nodeList* nList) {
  if (nList->capacity == 0) {
    nList->nodes = cmaken(NODE_LIST_INIT_CAP, nodeWrapper);
    postMalloc(nList->nodes);
    nList->capacity = NODE_LIST_INIT_CAP;
  } else {
    nList->nodes = realloc(
      nList->nodes,
      nList->capacity * 2);
    postMalloc(nList->nodes);
    nList->capacity *= 2;
  }

  return nList;
}

void nl_push (nodeList* nList, nodeWrapper node) {
  if (nList->capacity < nList->capacity + sizeof(node)) {
    nList = nl_grow(nList);

    if (nList->nodes == NULL) {
      printf(
        "ERROR - Could not push to nodeList: no memory");
      return;
    }
  }

  nList->nodes[nList->length] = node;
  nList->offsetPtr = &nList->nodes[nList->length];
  nList->length += 1;
}

nodeWrapper* nl_nextNode (nodeList* nList) {
  if (nList->iteratorPos >= nList->length)
    return NULL;

  nodeWrapper* node = &nList->nodes[nList->iteratorPos];
  nList->iteratorPos++;

  return node;
}

nodeWrapper* nl_iterator (nodeList* nList) {
  return (nList->nodes + nList->iteratorPos);
}

void nl_resetIterator (nodeList* nList) {
  nList->iteratorPos = 0;
}

nodeWrapper* nl_get (nodeList* nList, uint offset) {
  if ((offset + 1) > nList->length) return NULL;

  return (nList->nodes + offset);
}

