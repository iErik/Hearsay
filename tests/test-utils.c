#include <stdlib.h>
#include <stdio.h>

#include "utils/text.h"
#include "tests/test-utils.h"


void vfail (const cstring message, va_list args) {
  vfprintf(stdout, message, args);
  abort();
}

void fail (const cstring message, ...) {
  va_list args;

  va_start(args, message);
  vfprintf(stdout, message, args);
  va_end(args);

  abort();
}

void testing (const cstring what) {
  printr(interpol("=== %s ===", what), (renderTextOpts) {
    .bold      = true,
    .underline = true,
    .fgColor   = FG_Black,
    .bgColor   = BG_Cyan
  });

  printf("\n");
}

void expect (
  int condition,
  const cstring message,
  ...
) {
  va_list args;
  va_start(args, message);

  cstring output = vinterpol(message, args);
  cstring prefix = "";

  if (condition) {
    prefix = "[SUCCESS]";
    prefix = renderstr(prefix, (renderTextOpts) {
      .bold    = true,
      .fgColor = FG_Black,
      .bgColor = BG_BrightGreen,
    });

    printf("%s: ", prefix);
    printr(output, (renderTextOpts) {
      .bold = true,
      .fgColor = FG_BrightGreen,
    });
  } else {
    prefix = "[FAILURE]";
    prefix = renderstr(prefix, (renderTextOpts) {
      .bold    = true,
      .fgColor = FG_Black,
      .bgColor = BG_BrightRed,
    });

    printf("%s: ", prefix);
    printr(output, (renderTextOpts) {
      .bold = true,
      .fgColor = FG_BrightRed,
    });

    //abort();
  }

  va_end(args);
}

bool int_eq (int left, int right) {
  return left == right;
}

bool char_eq (char left, char right) {
  return left == right;
}

bool float_eq (float left, float right) {
  return left == right;
}

bool double_eq (double left, double right) {
  return left == right;
}

bool str_eq (char* left, char* right) {
  return streq(left, right);
}

bool sizet_eq (size_t left, size_t right) {
  return left == right;
}

bool ptr_eq (void* left, void* right) {
  return left == right;
}

