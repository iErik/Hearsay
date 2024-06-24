#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "lexer/lexer.h"


int isDigitSeparator (lexer* lex) {
  if (lex->ch != '_') return 0;

  return
    isDigit(lookBehind(lex)) ||
    isDigit(lookAhead(lex));
}

// -> Functions
// ------------

lexer* mkLexer (cstring input) {
  lexer* lex = (lexer*) calloc(1, sizeof(lexer));

  lex->input = input;
  lex->inputLen = strlen(input);
  lex->position = 0;
  lex->lastPosition = 0;
  lex->readPosition = 0;
  lex->ch = 0;

  readChar(lex);
  return lex;
}

void destroyLexer (lexer* lex) {
  free(lex->input);
  free(lex);
}

token nextToken (lexer* lex) {
  skipWhitespace(lex);
  token tkn;

  switch (lex->ch) {
    case '(':
      tkn = newToken(TknLParen, &lex->ch);
      break;
    case ')':
      tkn = newToken(TknRParen, &lex->ch);
      break;
    case '{':
      tkn = newToken(TknLBrace, &lex->ch);
      break;
    case '}':
      tkn = newToken(TknRBrace, &lex->ch);
      break;
    case ';':
      tkn = newToken(TknSemicolon, &lex->ch);
      break;
    case ',':
      tkn = newToken(TknComma, &lex->ch);
      break;
    case '+':
      tkn = newToken(TknPlus, &lex->ch);
      break;
    case '-':
      tkn = newToken(TknMinus, &lex->ch);
      break;
    case '*':
      tkn = newToken(TknAsterisk, &lex->ch);
      break;
    case '/':
      tkn = newToken(TknSlash, &lex->ch);
      break;
    case '\\':
      tkn = newToken(TknBackslash, &lex->ch);
      break;
    case '<':
      tkn = newToken(TknLT, &lex->ch);
      break;
    case '>':
      tkn = newToken(TknGT, &lex->ch);
      break;
    case '=':
      if (lookAhead(lex) == '=') {
        tkn = newToken(TknEQ, "==");
        readChar(lex);
      } else {
        tkn = newToken(TknAssign, &lex->ch);
      }

      break;
    case '!':
      if (lookAhead(lex) == '=') {
        tkn = newToken(TknNEQ, "!=");
        readChar(lex);
      } else {
        tkn = newToken(TknBang, &lex->ch);
      }

      break;
    case '?':
      tkn = newToken(TknQuestion, &lex->ch);
    case '\0':
      tkn.literal = "";
      tkn.type = TknEOF;
      break;
    default:
      if (isLetter(lex->ch)) {
        tkn.literal = readIdentifier(lex);
        tkn.type = lookupIdent(tkn.literal);

        return tkn;
      }

      else if (isDigit(lex->ch)) {
        tkn.type = TknInt;
        tkn.literal = readNumber(lex);
        return tkn;
      }

      else { tkn = newToken(TknIllegal, &lex->ch); }
  }

  readChar(lex);
  return tkn;
}

void readChar (lexer* lexer) {
  if (lexer->readPosition >= lexer->inputLen)
    lexer->ch = 0;
  else
    lexer->ch = lexer->input[lexer->readPosition];

  lexer->lastPosition = lexer->position;
  lexer->position = lexer->readPosition;
  lexer->readPosition += 1;
}

int reachedEOF (lexer* lex) {
  return lex->ch == '\0';
}

char lookAhead (lexer* lex) {
  if (lex->readPosition >= lex->inputLen) return 0;

  return *(lex->input + lex->readPosition);
}

char lookBehind (lexer *lex) {
  if (lex->lastPosition >= lex->inputLen) return 0;

  return *(lex->input + lex->lastPosition);
}

void skipWhitespace (lexer* lex) {
  while (isWhitespace(lex->ch)) readChar(lex);
}

char* readNumber (lexer* lex) {
  int position = lex->position;

  while (isDigit(lex->ch) || isDigitSeparator(lex))
    readChar(lex);

  return substr(lex->input, position, lex->position);
}

char* readIdentifier (lexer* lex) {
  int position = lex->position;

  while (isLetter(lex->ch))
    readChar(lex);

  return substr(lex->input, position, lex->position);
}
