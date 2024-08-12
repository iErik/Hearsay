#define DEBUG

#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "tests/test-utils.h"
#include "utils.h"
#include "lexer/lexer.h"


// -> Types Defs
// -------------

typedef struct tokenExpect {
  tokenType type;
  cstring   literal;
} tokenExpect;



// -> Test Fns
// -----------

void TestReadIdentifier () {
  printf("\n=== ReadIdentifier Test ===\n");

  char* testStr = "This is a test";
  char* testSubstr = subnstr(testStr, 10, 4);

  expect(strcmp(testSubstr, "test") == 0,
    "\nTest failed - testSubstr mismatch.\n"
    "Expected: %s\n"
    "Received: %s\n",
    "test", testSubstr);

  expect(strlen(testSubstr) == 4,
    "\nTest failed - testSubstr length mismatch.\n"
    "Expected: %i\n"
    "Received: %i\n",
    4, strlen(testSubstr));

  free(testSubstr);

  testSubstr = subnstr(testStr, 5, 4);

  expect(strcmp(testSubstr, "is a") == 0,
    "\nTest failed - testSubstr mismatch.\n"
    "Expected: %s\n"
    "Received: %s\n",
    "is a", testSubstr);

  expect(strlen(testSubstr) == 4,
    "\nTest failed - testSubstr length mismatch.\n"
    "Expected: %i\n"
    "Received: %i\n",
    4, strlen(testSubstr));

  printf("=== ReadIdentifier Test Passed ===\n");

  free(testSubstr);
}

void TestMkLexer () {
  printf("\n=== mkLexer Test ===\n");

  cstring input = "=+(){};abcsd";
  lexer* lex = mkLexer(input);

  expect(strcmp(lex->input, input) == 0,
    "\nTest failed - Lexer input mismatch.\n"
    "Expected: %s\n"
    "Received: %s\n",
    input, lex->input);

  int inputLen = strlen(input);

  expect(lex->inputLen == inputLen,
    "\nTest failed - Lexer input length mismatch.\n"
    "Expected: %i\n"
    "Received: %i\n",
    inputLen, lex->inputLen);

  expect(lex->position == 0,
    "\nTest failed - Lexer position mismatch.\n"
    "Expected: %i\n"
    "Received: %i\n",
    0, lex->position);

  expect(lex->readPosition == 1,
    "\nTest failed - Lexer read position mismatch.\n"
    "Expected: %i\n"
    "Received: %i\n",
    1, lex->readPosition);

  expect(lex->ch == *input,
    "\nTest failed - Lexer character mismatch.\n"
    "Expected: %c\n"
    "Received: %c\n",
    *input, lex->ch);

  free(lex);
  printf("\nTests completed sucessfuly.\n");
}

void TestNextToken () {
  printf("\n=== nextToken Test ===\n");

  cstring input =
    "let five = 5;\n"
    "let ten = 10;\n"

    "let add = fun (x, y) {\n"
    "  x + y;\n"
    "};\n"

    "let result = add(five, ten);"

    "!-/*5;"
    "5 < 10 > 5;"

    "if (5 != 10) { ret true; }"
    "elif (5 == 10) { ret 20; }"
    "else { ret false; }";

  token token;

  tokenExpect tests[] =  {
    { TknLet, "let" },
    { TknIdent, "five" },
    { TknAssign, "=" },
    { TknInt, "5" },
    { TknSemicolon, ";" },
    { TknLet, "let" },
    { TknIdent, "ten" },
    { TknAssign, "=" },
    { TknInt, "10" },
    { TknSemicolon, ";" },
    { TknLet, "let" },
    { TknIdent, "add" },
    { TknAssign, "=" },
    { TknFunction, "fun" },
    { TknLParen, "(" },
    { TknIdent, "x" },
    { TknComma, "," },
    { TknIdent, "y" },
    { TknRParen, ")" },
    { TknLParen, "{" },
    { TknIdent, "x" },
    { TknPlus, "+" },
    { TknIdent, "y" },
    { TknSemicolon, ";" },
    { TknRBrace, "}" },
    { TknSemicolon, ";" },
    { TknLet, "let" },
    { TknIdent, "result" },
    { TknAssign, "=" },
    { TknIdent, "add" },
    { TknLParen, "(" },
    { TknIdent, "five" },
    { TknComma, "," },
    { TknIdent, "ten" },
    { TknRParen, ")" },
    { TknSemicolon, ";" },
    { TknBang, "!" },
    { TknMinus, "-" },
    { TknSlash, "/"},
    { TknAsterisk, "*" },
    { TknInt, "5" },
    { TknSemicolon, ";" },
    { TknInt, "5" },
    { TknLT, "<" },
    { TknInt, "10" },
    { TknGT, ">" },
    { TknInt, "5" },
    { TknSemicolon, ";" },
    { TknIf, "if" },
    { TknLParen, "(" },
    { TknInt, "5" },
    { TknNEQ, "!=" },
    { TknInt, "10" },
    { TknRParen, ")" },
    { TknLBrace, "{" },
    { TknReturn, "ret" },
    { TknTrue, "true" },
    { TknSemicolon, ";" },
    { TknRBrace, "}" },
    { TknElif, "elif" },
    { TknLParen, "(" },
    { TknInt, "5" },
    { TknEQ, "==" },
    { TknInt, "10" },
    { TknRParen, ")" },
    { TknLBrace, "{" },
    { TknReturn, "ret" },
    { TknInt, "20" },
    { TknSemicolon, ";" },
    { TknRBrace, "}" },
    { TknElse, "else" },
    { TknLBrace, "{" },
    { TknReturn, "ret" },
    { TknFalse, "false" },
    { TknSemicolon, ";" },
    { TknRBrace, "}" },
    { TknEOF, "" }
  };

  lexer* lex = mkLexer(input);
  size_t testsLen = sizeof(tests) / sizeof(tokenExpect);

  for (int i = 0; i < testsLen; i++) {
    token = nextToken(lex);

    expect(token.type == tests[i].type,
      "\nTest failed - incorrect token type.\n"
      "Expected: %s\n"
      "Received: %s\n"
      "Position: %i, Character: %c\n",
      tests[i].type, token.type,
      lex->position, lex->ch);

    expect(strcmp(token.literal, tests[i].literal) == 0,
      "\nTest failed - incorrect token literal for: %s.\n"
      "Expected: %s\n"
      "Received: %s\n"
      "Position: %i, Character: %c\n",
      tests[i].type,
      tests[i].literal, token.literal,
      lex->position, lex->ch);
  }

  free(lex);
  printf("\nTests completed sucessfuly.\n");
}

// -> Main
// -------

int main () {
  TestReadIdentifier();
  TestMkLexer();
  TestNextToken();

  return 0;
}
