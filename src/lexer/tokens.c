#include <stdlib.h>
#include <string.h>

#include "lexer/tokens.h"


// -> Constants
// ------------

const token KEYWORDS[] = {
  { .type = TknFunction, .literal = "fun" },
  { .type = TknLet,      .literal = "let" },
  { .type = TknReturn,   .literal = "ret" },
  { .type = TknIf,       .literal = "if" },
  { .type = TknElif,     .literal = "elif" },
  { .type = TknElse,     .literal = "else" },
  { .type = TknTrue,     .literal = "true" },
  { .type = TknFalse,    .literal = "false" },
};

// -> Functions
// ------------

bool isLetter (char ch) {
  return
    ('a' <= ch && ch <= 'z') ||
    ('A' <= ch && ch <= 'Z') ||
    (ch == '_' || ch == '-') ||
    (ch == '?' || ch == '!');
}

bool isDigit (char ch) {
  return '0' <= ch && ch <= '9';
}

bool isWhitespace (char ch) {
  return
    ch ==  ' ' || ch == '\t' ||
    ch == '\n' || ch == '\r';
}

bool tokenIs (token tkn, tokenType type) {
  return tkn.type == type;
}

tokenType lookupIdent (cstring identifier) {
  size_t keyLen = sizeof(KEYWORDS) / sizeof(token);

  for (size_t i = 0; i < keyLen; i++) {
    if (strcmp(identifier, KEYWORDS[i].literal) == 0)
      return KEYWORDS[i].type;
  }

  return TknIdent;
}

token newToken (tokenType type, char* literal) {
  token tkn = { type };
  int length = strlen(literal) + 1;
  tkn.literal = (cstring) malloc(length * sizeof(char));

  memset(tkn.literal, '\0', length);
  strcpy(tkn.literal, literal);

  return tkn;
}

