#pragma once

#include <stdarg.h>

#include "common.h"


#define Test (name) \
  void func () {    \
    testing(name);  \
  }

void vfail (const cstring message, va_list args);

void fail (const cstring emssage, ...);

void testing (const cstring what);

void expect (
  int condition,
  const cstring errorMessage,
  ...
);
