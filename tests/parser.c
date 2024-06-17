#include "string.h"

#include "common.h"

#include "lexer.h"
#include "parser/parser.h"

#include "test-utils.h"

bool testLetStatement (
  statementNode node,
  cstring expectedId
) {
  letStatement* nFields = (letStatement *) node.fields;

  expect(node.type == LetStatement,
    "Wrong statement type\n"
    "Expected: %i\n"
    "Received: %i",
    LetStatement, node.type);


  /*
  if (strcmp(tkn->literal, "let") != 0) { }

  if (strcmp(ident->value, name) != 0) { }

  if (strcmp(ident->token.literal, name) != 0) {}
  */

  return true;
}

void TestLetStatements () {
  cstring input =
    "let x = 5;"
    "let y = 10;"
    "let foo = 9999;";

  lexer* lex = mkLexer(input);
  parser* pars = mkParser(lex);
  programNode* prog = parseProgram(pars);

  expect(prog != NULL, "Program Node is NULL!\n");

  expect(prog->statements->length == 3,
    "Wrong number of statements parsed\n"
    "Expected: %i\n"
    "Received: %i\n",
    3, prog->statements->length);

  cstring expectIds[] = { "x", "y", "foo" };

  for (int i = 0; i < 3; i++) testLetStatement(
    prog->statements->nodes[i],
    expectIds[i]);
}


int main () {
  TestLetStatements();

  return 0;
}
