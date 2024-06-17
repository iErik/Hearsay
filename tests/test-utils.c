#include <stdlib.h>
#include <stdio.h>

#include "test-utils.h"

void vfail (const char* message, va_list args) {
  vfprintf(stdout, message, args);
  abort();
}

void fail (const char* message, ...) {
  va_list args;

  va_start(args, message);
  vfprintf(stdout, message, args);
  va_end(args);

  abort();
}

void expect (
  int condition,
  const char* errorMessage,
  ...
) {
  if (condition) return;

  va_list args;

  va_start(args, errorMessage);
  vfail(errorMessage, args);
  va_end(args);
}
