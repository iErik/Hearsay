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

// ----------------------------------------------------- //
// -> Integer / Boolean Literals                         //
// ----------------------------------------------------- //

void TestLiterals () {
  testing("Language Literals");

  cstring input =
    "1230;"
    "0;"
    "23.4;"
    "true;"
    "false;";

  lexer* lex = mkLexer(input);
  parser* pars = mkParser(lex);
  rootNode* prog = parseProgram(pars);

  checkParserErrors(pars);

  expectEq(prog->nodes->length, 5,
    "Program must have 5 nodes");

  nodeWrapper* firstNode = getNode(prog, 0);
  expectEq((int) firstNode->type, (int) IntegerLiteral,
    "First node's type should be IntegerLiteral");

  nodeWrapper* secNode = getNode(prog, 1);
  expectEq((int) secNode->type, (int) IntegerLiteral,
    "Second node's type should be IntegerLiteral");

  nodeWrapper* thirdNode = getNode(prog, 2);
  expectEq((int) thirdNode->type, (int) IntegerLiteral,
    "Third node's type should be IntegerLiteral");

  nodeWrapper* fourthNode = getNode(prog, 3);
  expectEq((int) fourthNode->type, (int) BooleanLiteral,
    "Fourth node's type should be BooleanLiteral");

  nodeWrapper* fifthNode = getNode(prog, 4);
  expectEq((int) fifthNode->type, (int) BooleanLiteral,
    "Fifith node's type should be BooleanLiteral");
}

// ----------------------------------------------------- //
// -> Function Literals                                  //
// ----------------------------------------------------- //

void TestFunctionLiterals () { }

// ----------------------------------------------------- //
// -> If/Else Expressions                                //
// ----------------------------------------------------- //

void TestIfElse () { }

// ----------------------------------------------------- //
// -> Infix Expressions                                  //
// ----------------------------------------------------- //

void TestInfixExpr () {
  testing("Infix Expressions");

  cstring input =
    "1 + 3;"
    "(5 + 5) * 2;"
    "3 + 4 * 7;";

  nodeList* nodeExp = nl_new();

  nl_push(nodeExp,
    WrapNode(mkInfixNode(
      newToken(TknPlus, "+"),
      WrapNode(mkIntNode(1)),
      WrapNode(mkIntNode(3))
    )));

  nl_push(nodeExp,
    WrapNode(mkInfixNode(
      newToken(TknAsterisk, "*"),
      WrapNode(mkInfixNode(
        newToken(TknPlus, "+"),
        WrapNode(mkIntNode(5)),
        WrapNode(mkIntNode(5)))),
      WrapNode(mkIntNode(2))
    )));

  nl_push(nodeExp,
    WrapNode(mkInfixNode(
      newToken(TknInt, "+"),
      WrapNode(mkIntNode(3)),
      WrapNode(mkInfixNode(
        newToken(TknAsterisk, "*"),
        WrapNode(mkIntNode(4)),
        WrapNode(mkIntNode(7))))
    )));

  nodeList* parsedInput = parse(input);

  /*
  infixExpr exprExpect[] = {
    {
      .token = (token) {
        .literal = "1",
        .type = TknInt
      },
      .left = (nodeWrapper) {
        .type = IntegerLiteral,
        .node = &((integerLiteral) {
          .value = 1,
          .token = (token) {
            .literal = "1",
            .type = TknInt
          }
        })
      },
      .right = (nodeWrapper) {
        .type = IntegerLiteral,
        .node = &((integerLiteral) {
          .value = 1,
          .token = (token) {
            .literal = "1",
            .type = TknInt
          }
        })
      }
    },

    {
      .token = (token) {
        .literal = "(",
        .type = TknIdent
      },
      .left = (nodeWrapper) { },
      .right = (nodeWrapper) { },
    }
  };
  */

  expect(parsedInput != NULL,
      "Parsed input must not be NULL\n");
  expectEq(parsedInput->length, nodeExp->length,
    "Parsed input length");

  if (parsedInput->length != nodeExp->length)
    return;

  for (int i = 0; i < nodeExp->length; i++)
    expectEq(
      (int) CompareNodes(
        nl_get(parsedInput, i),
        nl_get(nodeExp, i)),
      true,
      "Node strucutre should match");
}

// ----------------------------------------------------- //
// -> Prefix Expressions                                 //
// ----------------------------------------------------- //

void TestPrefixExpr () {
  testing("Prefix Expressions");

  cstring input = "";

  lexer* lex = mkLexer(input);
  parser* pars = mkParser(lex);
  rootNode* program = parseProgram(pars);
}

// ----------------------------------------------------- //
// -> Return Statements                                  //
// ----------------------------------------------------- //

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

// ----------------------------------------------------- //
// -> Let Statements                                     //
// ----------------------------------------------------- //

bool testLetStatement (
  nodeWrapper* wrapper,
  cstring expectedId,
  double expectedVal
) {
  letStatement* letNode = (letStatement *) wrapper->node;
  identifierNode* idNode = letNode->name;
  token letTkn = letNode->token;
  token idTkn  = idNode->token;

  integerLiteral* val = (integerLiteral *)
    letNode->value.node;

  expectEq(val->value, expectedVal,
    "Let statement's and expectedVal");

  expectEq((int) wrapper->type, (int) LetStatement,
    "NodeWrapper's type must be LetStatement");

  expectEq((int) letTkn.type, (int) TknLet,
    "Statement's token.type must be TknLet");
  expectEq(letTkn.literal, "let",
    "Statement's token.literal must be 'let'");

  expectEq(idNode->value, expectedId,
    "Identifier's value and expectedId parameter");

  expectEq(idTkn.literal, expectedId,
    "Identifier's token literal");
  expectEq((int) idTkn.type, (int) TknIdent,
    "Identifier's token type");

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
  expect(prog != NULL, "Program Node must not be NULL\n");

  expectEq(prog->nodes->length, 3,
    "Number of statements parsed");

  cstring expectIds[] = { "x", "y", "foo" };
  double expectedValues[] = { 5, 10, 9999 };

  for (int i = 0; i < 3; i++) testLetStatement(
    getNode(prog, i), expectIds[i], expectedValues[i]);
}

int main () {
  TestLetStatements();
  TestInfixExpr();
  //TestLiterals();

  return 0;
}
