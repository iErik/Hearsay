#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "parser/nodes.h"
#include "utils/text.h"
#include "utils/mem.h"


// ----------------------------------------------------- //
// -> Root Node                                          //
// ----------------------------------------------------- //

void pushNode (rootNode* rNode, nodeWrapper node) {
  nl_push(rNode->nodes, node);
}

void* growRootNode (rootNode* rNode) {
  return nl_grow(rNode->nodes);
}

nodeWrapper* nextNode (rootNode* rNode) {
  return nl_nextNode(rNode->nodes);
}

nodeWrapper* iterator (rootNode* rNode) {
  return nl_iterator(rNode->nodes);
}

void resetIterator (rootNode* rNode) {
  return nl_resetIterator(rNode->nodes);
}

nodeWrapper* getNode (rootNode* rNode, uint offset) {
  return nl_get(rNode->nodes, offset);
}

bool compareNodeList (nodeList* left, nodeList* right) {
  if (left->length != right->length) return false;

  for (int i = 0; i < left->length; i++) {
    if (!CompareNodes(nl_get(left, i), nl_get(right, i)))
      return false;
  }

  return true;
}

// -> rootNode
// ===========

rootNode* mkRootNode () {
  rootNode* root = make(rootNode);
  postMalloc(root);

  root->nodes = nl_new();
  postMalloc(root->nodes);

  return root;
}

cstring rootAsString (rootNode* rNode) {
  cstring str;
  nodeWrapper* currNode = NULL;

  while ((currNode = nextNode(rNode)) != NULL)
    str = interpol("%s%s", str, wrapperAsString(currNode));

  return str;
}

nodeWrapper wrapRootNode (rootNode* node) {
  return (nodeWrapper) {
    .type = RootNode,
    .node = node
  };
}

void unmkRootNode (rootNode** rNode) {
  if (rNode == NULL) return;

  nl_destroy(&(*rNode)->nodes);
  sFree(rNode);
}

bool compareRoot (rootNode* left, rootNode* right) {
  return compareNodeList(left->nodes, right->nodes);
}

cstring describeRoot (rootNode* node) { }

// -> letStatement
// ===============

letStatement* mkLetStatement (token id, nodeWrapper val) {
  letStatement* node = make(letStatement);

  node->name = mkIdNode(id);
  node->token = (token) {
    .type = TknLet,
    .literal = "let"
  };

  return node;
}

cstring letAsString (letStatement* node) {
  if (node == NULL) return "";

  cstring literal = node->token.literal;

  cstring str = interpol("%s %s = %s;",
    literal,
    AsString(node->name),
    AsString(&node->value));

  return str;
}

nodeWrapper wrapLetNode (letStatement* node) {
  return (nodeWrapper) {
    .type = LetStatement,
    .node = node
  };
}

void unmkLetNode (letStatement** node) {
  unmkIdNode(&(*node)->name);
  FreeNode((*node)->value);
  sFree(node);
}

bool compareLet (letStatement* left, letStatement* right) {
  return compareId(left->name, right->name)
      && CompareNodes(&left->value, &right->value);
}

cstring describeLet (letStatement* left) { }

// -> identifierNode
// =================

identifierNode* mkIdNode (token tkn) {
  if (tkn.type != TknIdent) return NULL;

  identifierNode* node = make(identifierNode);
  postMalloc(node);

  node->token = tkn;
  node->value = tkn.literal;

  return node;
}

cstring identifierAsString (identifierNode* node) {
  if (node == NULL) return "";

  return node->value;
}

nodeWrapper wrapIdentifierNode (identifierNode* node) {
  return (nodeWrapper) {
    .type = IdentifierNode,
    .node = node
  };
}

void unmkIdNode (identifierNode** node) {
  sFree(node);
}

bool compareId (
  identifierNode* left,
  identifierNode* right
) {
  return tokenEq(&left->token, &right->token)
      && streq(left->value, right->value);
}

cstring describeId (identifierNode* node) { }

// -> fnExpr
// =========

fnExpr* mkFnNode (nodeWrapper params, nodeWrapper body) {
  fnExpr* node = make(fnExpr);
  postMalloc(node);

  node->params = params;
  node->body = body;

  return node;
}

cstring fnAsStr (fnExpr* fun) {
  return "";
}

nodeWrapper wrapFnNode (fnExpr* fun) {
  return (nodeWrapper) {
    .type = FunctionExpression,
    .node = fun
  };
}

void unmkFnNode (fnExpr** node) {
  FreeNode((*node)->params);
  FreeNode((*node)->body);
  sFree(node);
}

bool compareFn (fnExpr* left, fnExpr* right) {
  return tokenEq(&left->token, &right->token)
      && CompareNodes(&left->params, &right->params)
      && CompareNodes(&left->body, &right->body);
}

cstring describeFn (fnExpr* node) { }

// -> blockStatement
// =================

blockStatement* mkBlockStatement (nodeList* statements) {
  blockStatement* node = make(blockStatement);
  postMalloc(node);

  node->statements = statements;

  return node;
}

cstring blockAsStr (blockStatement* block) {
  nodeList* nodes = block->statements;
  nodeWrapper* currNode = NULL;
  cstring str;

  while ((currNode = nl_nextNode(nodes)) != NULL)
    str = interpol("%s%s", str, wrapperAsString(currNode));

  return str;
}

nodeWrapper wrapBlock (blockStatement* block) {
  return (nodeWrapper) {
    .type = BlockStatement,
    .node = block
  };
}

void unmkBlockNode (blockStatement** node) {
  nl_destroy(&(*node)->statements);
  sFree(node);
}

bool compareBlock (
  blockStatement* left,
  blockStatement* right
) {
  return compareNodeList(
    left->statements,
    right->statements);
}

cstring describeBlock (blockStatement* node) {}

// -> retStatement
// ===============

retStatement* mkRetStatement (nodeWrapper val) {
  retStatement* node = make(retStatement);

  node->value = val;
  node->token = (token) {
    .literal = "return",
    .type = TknReturn
  };

  return node;
}

cstring returnAsString (retStatement* node) {
  if (node == NULL) return "";

  cstring literal = node->token.literal;

  cstring str = interpol("%s %s;",
    literal,
    AsString(&node->value));

  return str;
}

nodeWrapper wrapReturnNode (retStatement* node) {
  return (nodeWrapper) {
    .type = ReturnStatement,
    .node = node
  };
}

void unmkReturnNode (retStatement** node) {
  FreeNode((*node)->value);
  sFree(node);
}

bool compareReturn (
  retStatement* left,
  retStatement* right
) {
  return tokenEq(&left->token, &right->token)
      && CompareNodes(&left->value, &right->value);
}

cstring describeReturn (retStatement* node) { }

// -> ifExpr
// =========

ifExpr* mkIfNode (
  token tkn,
  nodeWrapper cond,
  nodeWrapper body,
  nodeWrapper elseB
) {
  ifExpr* node = make(ifExpr);

  node->token = tkn;
  node->condition = cond;
  node->body = body;
  node->elseBlock = elseB;

  return node;
}

cstring ifAsStr (ifExpr* node) {
  cstring nodeStr = interpol(
    "if (%s) {\n",
    AsString(&node->condition));

  nodeList* body =
    ((blockStatement*) node->body.node)->statements;

  for (int i = 0; i < body->length; i++) {
    nodeStr = interpol("%s%s\n",
      nodeStr, AsString(&body->nodes[i]));
  }

  nodeStr = interpol("%s}\n", nodeStr);

  return nodeStr;
}

nodeWrapper wrapIfNode (ifExpr* node) {
  return (nodeWrapper) {
    .type = IfExpression,
    .node = node
  };
}

void unmkIfNode (ifExpr** nodePtr) {
  ifExpr* node = *nodePtr;

  FreeNode(node->condition);
  FreeNode(node->body);
  FreeNode(node->elseBlock);

  sFree(nodePtr);
}

bool compareIf (ifExpr* left, ifExpr* right) {
  return tokenEq(&left->token, &right->token)
      && CompareNodes(&left->condition, &right->condition)
      && CompareNodes(&left->body, &right->body)
      && CompareNodes(&left->elseBlock, &right->elseBlock);
}

cstring describeIf (ifExpr* node) { }

// -> prefixExpr
// =============

prefixExpr* mkPrefixNode (
  token operator,
  nodeWrapper right
) {
  prefixExpr* node = make(prefixExpr);
  postMalloc(node);

  node->token    = operator;
  node->operator = operator.literal;

  node->right = right;

  return node;
}

cstring prefixAsStr (prefixExpr* node) {
  return interpol("%s%s;",
    node->operator, AsString(&node->right));
}

nodeWrapper wrapPrefixNode (prefixExpr* node) {
  return (nodeWrapper) {
    .type = PrefixExpression,
    .node = node
  };
}

void unmkPrefixNode (prefixExpr** node) {
  FreeNode((*node)->right);
  sFree(node);
}

bool comparePrefix (prefixExpr* left, prefixExpr* right) {
  return tokenEq(&left->token, &right->token)
      && streq(left->operator, right->operator)
      && CompareNodes(&left->right, &right->right);
}

cstring describePrefix (prefixExpr* node) { }

// -> infixExpr
// ============

infixExpr* mkInfixNode (
  token operator,
  nodeWrapper left,
  nodeWrapper right
) {
  infixExpr* node = make(infixExpr);

  node->token = operator;
  node->operator = operator.literal;
  node->left = left;
  node->right = right;

  return node;
}

cstring infixAsStr (infixExpr* node) {
  return interpol("%s %s %s",
    AsString(&node->left),
    node->operator,
    AsString(&node->right));
}

nodeWrapper wrapInfixNode (infixExpr* node) {
  return (nodeWrapper) {
    .type = InfixExpression,
    .node = node
  };
}

void unmkInfixNode (infixExpr** node) {
  FreeNode((*node)->left);
  FreeNode((*node)->right);
  sFree(node);
}

bool compareInfix (infixExpr* left, infixExpr* right) {
  return tokenEq(&left->token, &right->token)
      && CompareNodes(&left->left, &right->left)
      && streq(left->operator, right->operator)
      && CompareNodes(&left->right, &right->right);
}

cstring describeInfix (infixExpr* node) { }

// -> boolExpr
// ===========

boolExpr* mkBoolNode (token tkn) {
  boolExpr* bNode = make(boolExpr);

  bNode->token = tkn;
  bNode->value = tkn.type == TknTrue;

  return bNode;
}

cstring boolAsStr (boolExpr* expr) {
  return expr->value ? "true" : "false";
}

nodeWrapper wrapBoolNode (boolExpr* expr) {
  return (nodeWrapper) {
    .type = BooleanLiteral,
    .node = expr
  };
}

void unmkBoolNode (boolExpr** node) {
  sFree(node);
}

bool compareBool (boolExpr* left, boolExpr* right) {
  return tokenEq(&left->token, &right->token)
      && left->value == right->value;
}

cstring describeBool (boolExpr* node) { }

// -> integerLiteral
// =================

integerLiteral* mkIntNode (double number) {
  integerLiteral* lit = make(integerLiteral);

  lit->value = number;
  lit->token = (token) {
    .type = TknInt,
    .literal = interpol("%d", (int) number)
  };

  return lit;
}

cstring intAsStr (integerLiteral* node) {
  return interpol("%d", node->value);
}

nodeWrapper wrapIntNode (integerLiteral* node) {
  return (nodeWrapper) {
    .type = IntegerLiteral,
    .node = node
  };
}

void unmkIntNode (integerLiteral** node) {
  sFree(&(*node)->token.literal);
  sFree(node);
}

bool compareInt(
  integerLiteral* left,
  integerLiteral* right
) {
  return tokenEq(&left->token, &right->token)
      && left->value == right->value;
}

cstring describeInt (integerLiteral* node) { }

// -> invalidNode
// ==============

invalidNode* mkInvalidNode (token tkn) {
  invalidNode* node = make(invalidNode);
  node->token = tkn;

  return node;
}

nodeWrapper wrapInvalidNode (invalidNode* node) {
  return (nodeWrapper) {
    .type = InvalidStatement,
    .node = node
  };
}

cstring invalidAsString (invalidNode* node) {
  if (node == NULL) return "";

  return node->token.literal;
}

void unmkInvalidNode (invalidNode** node) {
  sFree(node);
}

bool compareInvalid () { return true; }

cstring describeInvalid () { return ""; }

// -> nodeWrapper
// ==============

cstring wrapperAsString (nodeWrapper* wrapper) {
  if (wrapper == NULL || wrapper->node == NULL) return "";

  switch (wrapper->type) {
    case RootNode:
      return rootAsString((rootNode*) wrapper->node);
    case IdentifierNode:
      return identifierAsString(
        (identifierNode*) wrapper->node);
    case BooleanLiteral:
    case BooleanExpression:
      return boolAsStr((boolExpr*) wrapper->node);
    case IntegerLiteral:
      return intAsStr((integerLiteral*) wrapper->node);
    case InfixExpression:
      return infixAsStr((infixExpr*) wrapper->node);
    case FunctionExpression:
      return fnAsStr((fnExpr*) wrapper->node);
    case IfExpression:
      return ifAsStr((ifExpr*) wrapper->node);
    case PrefixExpression:
      return prefixAsStr((prefixExpr*) wrapper->node);
    case LetStatement:
      return letAsString((letStatement*) wrapper->node);
    case ReturnStatement:
      return returnAsString((retStatement*) wrapper->node);
    case BlockStatement:
      return blockAsStr((blockStatement*) wrapper->node);
    case InvalidStatement:
      return "<InvalidNode>";
  }
}

void unmkNodeWrapper (nodeWrapper* wrapper) {
  void* node = wrapper->node;

  switch (wrapper->type) {
    case RootNode:
      return unmkRootNode((rootNode**) &node);
    case IdentifierNode:
      return unmkIdNode((identifierNode**) &node);
    case BooleanLiteral:
    case BooleanExpression:
      return unmkBoolNode((boolExpr**) &node);
    case IntegerLiteral:
      return unmkIntNode((integerLiteral**) &node);
    case InfixExpression:
      return unmkInfixNode((infixExpr**) &node);
    case FunctionExpression:
      return unmkFnNode((fnExpr**) &node);
    case IfExpression:
      return unmkIfNode((ifExpr**) &node);
    case PrefixExpression:
      return unmkPrefixNode((prefixExpr**) &node);
    case LetStatement:
      return unmkLetNode((letStatement**) &node);
    case ReturnStatement:
      return unmkReturnNode((retStatement**) &node);
    case BlockStatement:
      return unmkBlockNode((blockStatement**) &node);
    case InvalidStatement:
      return unmkInvalidNode((invalidNode**) &node);
  }
}

bool compareNodeWrapper (
  nodeWrapper* left,
  nodeWrapper* right
) {
  if (left == NULL || right == NULL) return false;
  if (left->type != right->type) return false;

  switch (left->type) {
    case RootNode:
      return compareRoot(left->node, right->node);
    case LetStatement:
      return compareLet(left->node, right->node);
    case IdentifierNode:
      return compareId(left->node, right->node);
    case FunctionExpression:
      return compareFn(left->node, right->node);
    case BlockStatement:
      return compareBlock(left->node, right->node);
    case ReturnStatement:
      return compareReturn(left->node, right->node);
    case IfExpression:
      return compareIf(left->node, right->node);
    case PrefixExpression:
      return comparePrefix(left->node, right->node);
    case InfixExpression:
      return compareInfix(left->node, right->node);
    case BooleanLiteral:
    case BooleanExpression:
      return compareBool(left->node, right->node);
    case IntegerLiteral:
      return compareInt(left->node, right->node);
    case InvalidStatement:
      return 1;
  }

  return 0;
}

cstring describeNode (nodeWrapper* wrapper) {
  switch (wrapper->type) {
    case RootNode:
      return describeRoot(wrapper->node);
    case LetStatement:
      return describeLet(wrapper->node);
    case IdentifierNode:
      return describeId(wrapper->node);
    case FunctionExpression:
      return describeFn(wrapper->node);
    case BlockStatement:
      return describeBlock(wrapper->node);
    case ReturnStatement:
      return describeReturn(wrapper->node);
    case IfExpression:
      return describeIf(wrapper->node);
    case PrefixExpression:
      return describePrefix(wrapper->node);
    case InfixExpression:
      return describeInfix(wrapper->node);
    case BooleanLiteral:
    case BooleanExpression:
      return describeBool(wrapper->node);
    case IntegerLiteral:
      return describeInt(wrapper->node);
    case InvalidStatement:
      return describeInvalid();
  }

  return "";
}
