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
  parserNode* program = parseProgram(pars);
  checkParserErrors(pars);

}

bool testLetStatement (
  parserNode node,
  cstring expectedId
) {
  letStatement* letNode = (letStatement *) node.meta;
  parserNode idNode = (parserNode) letNode->name;
  token tkn = node.token;

  expect(node.type == LetStatement,
    "Statement should be of type LetStatement\n"
    "Expected: %i\n"
    "Received: %i\n",
    LetStatement, node.type);

  expect(tkn.type == TknLet,
    "Statement's token.type should be TknLet\n"
    "Expected: %i\n"
    "Received: %i\n",
    TknLet, tkn.type);
  expect(strcmp(tkn.literal, "let") == 0,
    "Statement's token.literal should be 'let'\n"
    "Expected: %s\n"
    "Received: %s\n",
    "let", tkn.literal);

  expect(strcmp(ident.value, expectedId) == 0,
    "Statement's ident.value should match id paramenter\n"
    "Expected: %s\n"
    "Received: %s\n",
    expectedId, ident.value);
  expect(strcmp(ident.token.literal, expectedId) == 0,
    "Statement ident.token.literal should match value\n"
    "Expected: %s\n"
    "Received: %s\n",
    expectedId, ident.token.literal);
  expect(ident.token.type == TknIdent,
    "Statement ident.token.literal should be TknIdent\n"
    "Expected: %i\n"
    "Received: %i\n",
    TknIdent, ident.token.type);

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
  parserNode* prog = parseProgram(pars);

  checkParserErrors(pars);
  expect(prog != NULL, "Program Node is NULL!\n");

  expect(prog->statements->length == 3,
    "Number of statements parsed should match\n"
    "Expected: %i\n"
    "Received: %i\n",
    3, prog->statements->length);

  cstring expectIds[] = { "x", "y", "foo" };

  for (int i = 0; i < 3; i++) {
    testLetStatement(
      prog->statements->nodes[i],
      expectIds[i]
    );
  }
}

int main () {
  TestLetStatements();

  return 0;
}
