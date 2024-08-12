#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "parser/nodes.h"
#include "utils/text.h"


// ----------------------------------------------------- //
// -> Root Node                                          //
// ----------------------------------------------------- //

rootNode* mkRootNode () {
  rootNode* root = make(rootNode);
  postMalloc(root);

  root->nodes = nl_new();
  postMalloc(root->nodes);

  return root;
}

void unmkRootNode (rootNode* rNode) { }

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

// -> rootNode
// ===========

cstring rootAsString (rootNode* rNode) {
  cstring str;
  nodeWrapper* currNode = NULL;

  while ((currNode = nextNode(rNode)) != NULL)
    str = interpol("%s%s", str, wrapperAsString(*currNode));

  return str;
}

cstring rootTokenLiteral (rootNode* rNode) {
  if (rNode->nodes->length == 0) return "";

  nodeWrapper* firstNode = getNode(rNode, 0);
  return getTokenLiteral(firstNode);
}

nodeWrapper wrapRootNode (rootNode* node) {
  return (nodeWrapper) {
    .type = RootNode,
    .node = node
  };
}

// -> letStatement
// ===============

letStatement* mkLetStatement (token letTkn, token idTkn) {
  if (letTkn.type != TknLet) return NULL;

  letStatement* node = make(letStatement);

  node->token = letTkn;
  node->name  = (identifierNode) {
    .token = idTkn,
    .value = idTkn.literal
  };

  return node;
}

cstring letAsString (letStatement* node) {
  if (node == NULL) return "";

  cstring literal = node->token.literal;

  cstring str = interpol("%s %s = %s;",
    literal,
    AsString(&node->name),
    AsString(node->value));

  return str;
}

nodeWrapper wrapLetNode (letStatement* node) {
  return (nodeWrapper) {
    .type = LetStatement,
    .node = node
  };
}

// -> retStatement
// ===============

retStatement* mkRetStatement (token retTkn) {
  if (retTkn.type != TknReturn) return NULL;

  retStatement* node = make(retStatement);

  node->token = retTkn;

  return node;
}

cstring returnAsString (retStatement* node) {
  if (node == NULL) return "";

  cstring literal = node->token.literal;

  cstring str = interpol("%s %s;",
    literal,
    AsString(node->value));

  return str;
}

nodeWrapper wrapReturnNode (retStatement* node) {
  return (nodeWrapper) {
    .type = ReturnStatement,
    .node = node
  };
}

// -> identifierNode
// =================

identifierNode* mkIdNode (token tkn) {
  if (tkn.type != TknIdent) return NULL;

  identifierNode* node = make(identifierNode);

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

// -> blockStatement
// =================

blockStatement* mkBlockStatement () { }

cstring blockAsStr (blockStatement* block) { }

nodeWrapper wrapBlock (blockStatement* block) {
  return (nodeWrapper) {
    .type = BlockStatement,
    .node = block
  };
}

// -> prefixExpr
// =============

prefixExpr* mkPrefixNode (
  token operator,
  nodeWrapper right
) {
  prefixExpr* node = make(prefixExpr);

  node->token    = operator;
  node->operator = operator.literal;

  node->right = right;

  return node;
}

cstring prefixAsStr (prefixExpr* node) {
  return interpol("%s%s;",
    node->operator, AsString(node->right));
}

nodeWrapper wrapPrefixNode (prefixExpr* node) {
  return (nodeWrapper) {
    .type = PrefixExpression,
    .node = node
  };
}

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
    AsString(node->left),
    node->operator,
    AsString(node->right));
}

nodeWrapper wrapInfixNode (infixExpr* node) {
  return (nodeWrapper) {
    .type = InfixExpression,
    .node = node
  };
}

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
    AsString(node->condition));

  nodeList* body =
    ((blockStatement*) node->body.node)->statements;

  for (int i = 0; i < body->length; i++) {
    nodeStr = interpol("%s%s\n",
      nodeStr, AsString(body->nodes[i]));
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

// -> boolExpr
// ===========

boolExpr* mkBoolNode (token tkn) {
  boolExpr* bNode = make(boolExpr);

  bNode->token = tkn;
  bNode->value = tkn.type == TknTrue;

  return bNode;
}

cstring boolAsStr (boolExpr* expr) { }

nodeWrapper wrapBoolNode (boolExpr* expr) {
  return (nodeWrapper) {
    .type = BooleanLiteral,
    .node = expr
  };
}

// -> fnExpr
// =========

fnExpr* mkFnNode () { }

cstring fnAsStr (fnExpr* fun) {

}

nodeWrapper wrapFnNode (fnExpr* fun) {
  return (nodeWrapper) {
    .type = FunctionExpression,
    .node = fun
  };
}

// -> integerLiteral
// =================

integerLiteral* mkIntNode () { }

cstring intAsStr (integerLiteral* num) { }

nodeWrapper wrapIntNode (integerLiteral* num) {
  return (nodeWrapper) {
    .type = IntegerLiteral,
    .node = num
  };
}

// -> exprStatement
// ================

nodeWrapper wrapExprNode (void* node) {
  return (nodeWrapper) {
    .type = ExpressionNode,
    .node = node
  };
}

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

// -> nodeWrapper
// ==============

cstring wrapperAsString (nodeWrapper wrapper) {
  if (wrapper.node == NULL) return "";

  switch (wrapper.type) {
    case RootNode:
      return rootAsString(wrapper.node);
    case IdentifierNode:
      return identifierAsString(wrapper.node);
    case LetStatement:
      return letAsString(wrapper.node);
    case ReturnStatement:
      return returnAsString(wrapper.node);
    case InvalidStatement:
    default:
      return "";
  }
}

cstring getTokenLiteral (nodeWrapper* wrapper) {
  void* node = wrapper->node;

  switch (wrapper->type) {
    case RootNode:
      return rootTokenLiteral(wrapper->node);
    case IdentifierNode:
      return ((identifierNode *) node)->token.literal;
    case LetStatement:
      return ((letStatement *) node)->token.literal;
    case ReturnStatement:
      return ((retStatement *) node)->token.literal;
    case InvalidStatement:
    default:
      return "";
  }
}

