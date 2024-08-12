#include "tests/test-utils.h"
#include "utils/text.h"
#include "parser/nodes.h"


// --------------------------------------------------------
// -> Let Statement
// --------------------------------------------------------

void TestLetStatement () {
  testing("LetStatement");

  token letTkn = { .literal = "let", .type = TknLet };
  token idTkn  = { .literal = "test", .type = TknIdent };

  letStatement* letNode = mkLetStatement(letTkn, idTkn);

  expect(letNode != NULL,
    "LetStatement node must not be NULL\n"
    "Received: %p\n", letNode);

  expect(letNode->name.token.type == idTkn.type,
    "LetStatement's name token type must be TknIdent\n"
    "Expected: %i\n"
    "Received: %i\n",
    idTkn.type, letNode->name.token.type);
  expect(streq(
    letNode->name.token.literal, idTkn.literal),
    "LetStatement's name token literal must match\n"
    "Expected: %s\n"
    "Received: %s\n",
    idTkn.literal, letNode->name.token.literal);
  expect(streq(
    letNode->name.value, idTkn.literal),
    "LetStatement's name value must match\n"
    "Expected: %s\n"
    "Received: %s\n",
    idTkn.literal, letNode->name.value);

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


// --------------------------------------------------------
// -> Return Statement
// --------------------------------------------------------

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


// --------------------------------------------------------
// -> Identifier Node
// --------------------------------------------------------

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


// --------------------------------------------------------
// -> Node Macros
// --------------------------------------------------------

void TestAsString () {
  testing("As String Node Macro");

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

void TestNodeWrapper () { }

// --------------------------------------------------------
// -> Expressions Node
// --------------------------------------------------------


// --------------------------------------------------------
// -> Main
// --------------------------------------------------------

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
