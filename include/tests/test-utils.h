#pragma once

#include <stdarg.h>
#include "common.h"
#include "utils/text.h"

bool int_eq (int left, int right);
bool sizet_eq (size_t left, size_t right);
bool char_eq (char left, char right);
bool str_eq (char* left, char* right);
bool ptr_eq (void* left, void* right);
bool float_eq (float left, float right);
bool double_eq (double left, double right);

#define TestIdentifier(Module, Name, Suffix) \
  _Test_ ## Module ## Name ## Suffix

#define TestPrototype(Module, Name, Suffix) \
  void TestIdentifier(Module, Name, Suffix)(void)

#define UnitTest(Module, Name) \
  TestPrototype(Module, Name, Impl); \
  TestPrototype(Module, Name, Impl)


#define PRINT_FORMAT(T)               \
  _Generic((T),                       \
    char              : "%c",         \
    signed char       : "%hhd",       \
    unsigned char     : "%hhu",       \
    short             : "%hd",        \
    int               : "%d",         \
    long              : "%ld",        \
    long long         : "%lld",       \
    unsigned short    : "%hu",        \
    unsigned int      : "%u",         \
    unsigned long     : "%lu",        \
    unsigned long long: "%llu",       \
    float             : "%f",         \
    double            : "%f",         \
    long double       : "%Lf",        \
    char*             : "%s",         \
    wchar_t*          : "%ls",        \
    void*             : "%p"          \
  )

#define EQ(left, right) _Generic((left),                  \
  int:     int_eq,                                        \
  size_t:  sizet_eq,                                      \
  char:    char_eq,                                       \
  char*:   str_eq,                                        \
  float:   float_eq,                                      \
  double:  double_eq,                                     \
  void*:   ptr_eq,                                        \
  default: ptr_eq                                         \
  )(left, right)

#define expectEq(left, right, stat) do {                  \
  if (EQ(left, right))                                    \
    expect(EQ(left, right), "Expect Equal: %s\n", stat);  \
  else                                                    \
    expect(EQ(left, right), interpol(                     \
      "Expect Equal: %s\n\n - Expected: %s\n%s", stat,    \
      PRINT_FORMAT( (left) ),                             \
      interpol(                                           \
        " - Received: %s\n",                              \
        PRINT_FORMAT( (right) ))                          \
    ), right, left);                                      \
} while (0)

void vfail (const cstring message, va_list args);

void fail (const cstring emssage, ...);

void testing (const cstring what);

void expect (
  int condition,
  const cstring errorMessage,
  ...
);

