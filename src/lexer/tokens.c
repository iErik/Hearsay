#include <stdlib.h>
#include <string.h>

#include "lexer/tokens.h"
#include "utils/text.h"


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

token newToken (tokenType type, char* literal) {
  token tkn = { type };
  int length = strlen(literal) + 1;
  tkn.literal = (cstring) malloc(length * sizeof(char));

  memset(tkn.literal, '\0', length);
  strcpy(tkn.literal, literal);

  return tkn;
}

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

bool tokenEq (token* left, token* right) {
  return left->type == right->type
      && streq(left->literal, right->literal);
}

opPrecedence tknPrecedence (token tkn) {
  switch (tkn.type) {
    case TknEQ:
    case TknNEQ:
      return PrecEquals;
    case TknLT:
    case TknGT:
      return PrecLessGreater;
    case TknPlus:
    case TknMinus:
      return PrecSum;
    case TknSlash:
    case TknAsterisk:
      return PrecProduct;
    default:
      return PrecLowest;
  }
}

tokenType lookupIdent (cstring identifier) {
  size_t keyLen = sizeof(KEYWORDS) / sizeof(token);

  for (size_t i = 0; i < keyLen; i++) {
    if (strcmp(identifier, KEYWORDS[i].literal) == 0)
      return KEYWORDS[i].type;
  }

  return TknIdent;
}


