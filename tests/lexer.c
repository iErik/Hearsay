#define DEBUG

#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "test-utils.h"
#include "lexer.h"


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
  printf("\nmkLexer test\n");
  printf("-----------\n");

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
  printf("\nnextToken test\n");
  printf("-----------\n");

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
    { TKN_LET, "let" },
    { TKN_IDENT, "five" },
    { TKN_ASSIGN, "=" },
    { TKN_INT, "5" },
    { TKN_SEMICOLON, ";" },
    { TKN_LET, "let" },
    { TKN_IDENT, "ten" },
    { TKN_ASSIGN, "=" },
    { TKN_INT, "10" },
    { TKN_SEMICOLON, ";" },
    { TKN_LET, "let" },
    { TKN_IDENT, "add" },
    { TKN_ASSIGN, "=" },
    { TKN_FUNCTION, "fun" },
    { TKN_LPAREN, "(" },
    { TKN_IDENT, "x" },
    { TKN_COMMA, "," },
    { TKN_IDENT, "y" },
    { TKN_RPAREN, ")" },
    { TKN_LBRACE, "{" },
    { TKN_IDENT, "x" },
    { TKN_PLUS, "+" },
    { TKN_IDENT, "y" },
    { TKN_SEMICOLON, ";" },
    { TKN_RBRACE, "}" },
    { TKN_SEMICOLON, ";" },
    { TKN_LET, "let" },
    { TKN_IDENT, "result" },
    { TKN_ASSIGN, "=" },
    { TKN_IDENT, "add" },
    { TKN_LPAREN, "(" },
    { TKN_IDENT, "five" },
    { TKN_COMMA, "," },
    { TKN_IDENT, "ten" },
    { TKN_RPAREN, ")" },
    { TKN_SEMICOLON, ";" },
    { TKN_BANG, "!" },
    { TKN_MINUS, "-" },
    { TKN_SLASH, "/"},
    { TKN_ASTERISK, "*" },
    { TKN_INT, "5" },
    { TKN_SEMICOLON, ";" },
    { TKN_INT, "5" },
    { TKN_LT, "<" },
    { TKN_INT, "10" },
    { TKN_GT, ">" },
    { TKN_INT, "5" },
    { TKN_SEMICOLON, ";" },
    { TKN_IF, "if" },
    { TKN_LPAREN, "(" },
    { TKN_INT, "5" },
    { TKN_NEQ, "!=" },
    { TKN_INT, "10" },
    { TKN_RPAREN, ")" },
    { TKN_LBRACE, "{" },
    { TKN_RET, "ret" },
    { TKN_TRUE, "true" },
    { TKN_SEMICOLON, ";" },
    { TKN_RBRACE, "}" },
    { TKN_ELIF, "elif" },
    { TKN_LPAREN, "(" },
    { TKN_INT, "5" },
    { TKN_EQ, "==" },
    { TKN_INT, "10" },
    { TKN_RPAREN, ")" },
    { TKN_LBRACE, "{" },
    { TKN_RET, "ret" },
    { TKN_INT, "20" },
    { TKN_SEMICOLON, ";" },
    { TKN_RBRACE, "}" },
    { TKN_ELSE, "else" },
    { TKN_LBRACE, "{" },
    { TKN_RET, "ret" },
    { TKN_FALSE, "false" },
    { TKN_SEMICOLON, ";" },
    { TKN_RBRACE, "}" },
    { TKN_EOF, "" }
  };

  lexer* lex = mkLexer(input);
  size_t testsLen = sizeof(tests) / sizeof(tokenExpect);

  for (int i = 0; i < testsLen; i++) {
    token = nextToken(lex);

    expect(strcmp(token.type, tests[i].type) == 0,
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
