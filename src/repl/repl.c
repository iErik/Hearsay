#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "repl/repl.h"

lexer* readLine (stream from) {
  char* input = (char*) malloc(INPUT_MAX * sizeof(char));
  memset(input, '\0', INPUT_MAX);
  fgets(input, INPUT_MAX, from);
  lexer* lex = mkLexer(input);

  return lex;
}

void startRepl (stream in, stream out) {

  while (1) {
    fprintf(out, "~ ");

    lexer* lex = readLine(in);
    token tkn;

    while (reachedEOF(lex) == FALSE) {
      tkn = nextToken(lex);
      fprintf(out, "{ Type: %s | Literal %s }\n",
        tkn.type, tkn.literal);
    }

    destroyLexer(lex);
  }
}
