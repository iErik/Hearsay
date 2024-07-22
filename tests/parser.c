#include "string.h"

#include "common.h"
#include "lexer/tokens.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

#include "tests/test-utils.h"

void checkParserErrors (parser* pars) {
  expect(!hasParsingErrors(pars),
    "Parser shouldn't have any parser errors\n"
    "Expected: [%s]\n"
    "Received: [%s]\n",
    "", listParserErrors(pars));
}

void TestReturnStatement () {
  cstring input =
    "return 5;"
    "return 10;"
    "return 993322;";

  lexer* lex = mkLexer(input);
  parser* pars = mkParser(lex);
  //rootNode* program = parseProgram(pars);

  checkParserErrors(pars);
}

bool testLetStatement (
  nodeWrapper* wrapper,
  cstring expectedId
) {
  letStatement* letNode = (letStatement *) wrapper->node;
  identifierNode* idNode = letNode->name;
  token letTkn = letNode->token;
  token idTkn  = idNode->token;

  expect(wrapper->type == LetStatement,
    "Statement should be of type LetStatement\n"
    "Expected: %i\n"
    "Received: %i\n",
    LetStatement, wrapper->type);

  expect(letTkn.type == TknLet,
    "Statement's token.type should be TknLet\n"
    "Expected: %i\n"
    "Received: %i\n",
    TknLet, letTkn.type);
  expect(strcmp(letTkn.literal, "let") == 0,
    "Statement's token.literal should be 'let'\n"
    "Expected: %s\n"
    "Received: %s\n",
    "let", letTkn.literal);

  expect(strcmp(idNode->value, expectedId) == 0,
    "Statement's ident.value should match id paramenter\n"
    "Expected: %s\n"
    "Received: %s\n",
    expectedId, idNode->value);
  expect(strcmp(idTkn.literal, expectedId) == 0,
    "Statement ident.token.literal should match value\n"
    "Expected: %s\n"
    "Received: %s\n",
    expectedId, idTkn.literal);
  expect(idTkn.type == TknIdent,
    "Statement ident.token.literal should be TknIdent\n"
    "Expected: %i\n"
    "Received: %i\n",
    TknIdent, idTkn.type);

  return true;
}

void TestLetStatements () {
  testing("LET Statements");

  cstring input =
    "let x = 5;"
    "let y = 10;"
    "let foo = 9999;";

  lexer* lex = mkLexer(input);
  parser* pars = mkParser(lex);
  rootNode* prog = parseProgram(pars);

  checkParserErrors(pars);
  expect(prog != NULL, "Program Node is NULL!\n");

  expect(prog->length == 3,
    "Number of statements parsed should match\n"
    "Expected: %i\n"
    "Received: %i\n",
    3, prog->length);

  cstring expectIds[] = { "x", "y", "foo" };

  for (int i = 0; i < 3; i++)
    testLetStatement(getNode(prog, i), expectIds[i]);
}

int main () {
  TestLetStatements();

  return 0;
}
