#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/text.h"

cstring vinterpol (const cstring message, va_list args) {
  va_list argsCopy;
  va_copy(argsCopy, args);

  size_t newSize = vsnprintf(
    NULL,
    0,
    message,
    argsCopy);

  va_end(argsCopy);

  if (newSize <= 0) return NULL;

  cstring newStr = (cstring) calloc(
    newSize + 1, sizeof(char));

  checkNullPtr(newStr);
  vsprintf(newStr, message, args);

  return newStr;
}

cstring interpol (const cstring str, ...) {
  va_list args;

  va_start(args, str);
  cstring newStr = vinterpol(str, args);
  va_end(args);

  return newStr;
}

cstring renderstr (
  const cstring text,
  renderTextOpts opts
) {
  return interpol("\e[0%s%s%s%s%sm%s\e[0m",
    opts.bold ? ";1" : "",
    opts.italic ? ";3" : "",
    opts.underline ? ";4" : "",
    opts.fgColor ? interpol(";%u", opts.fgColor) : "",
    opts.bgColor ? interpol(";%u", opts.bgColor) : "",
    text);
}

void printr (const cstring text, renderTextOpts opts) {
  puts(renderstr(text, opts));
}

char* substr (cstring src, uint begin, uint end) {
  if (strlen(src) < end) return 0;

  uint length = (end - begin) + 1;
  char* str = (char*) malloc(length * sizeof(char));
  checkNullPtr(str)

  memset(str, '\0', length);

  for (int i = begin; i < end; i++) {
    if (*(src + i) == '\0') break;

    *(str + (i - begin)) = *(src + i);
  }

  return str;
}

char* subnstr (cstring src, uint begin, uint count) {
  if (strlen(src) < (begin + count)) return 0;

  uint length = count + 1;
  char* str = (char*) malloc(length * sizeof(char));
  checkNullPtr(str)

  memset(str, '\0', length);

  for (int i = begin; i < (begin + count); i++) {
    if (*(src + i) == '\0') break;

    *(str + (i - begin)) = *(src + i);
  }

  return str;
}

bool streq (cstring fStr, cstring sStr) {
  return strcmp(fStr, sStr) == 0;
}
