#pragma once

#include <stdarg.h>

#include "common.h"

typedef enum {
  FG_Black   = 30,
  FG_Red     = 31,
  FG_Green   = 32,
  FG_Yellow  = 33,
  FG_Blue    = 34,
  FG_Magenta = 35,
  FG_Cyan    = 36,
  FG_White   = 37,
  FG_Default = 39,

  FG_BrightBlack   = 90,
  FG_BrightRed     = 91,
  FG_BrightGreen   = 92,
  FG_BrightYellow  = 93,
  FG_BrightBlue    = 94,
  FG_BrightMagenta = 95,
  FG_BrightCyan    = 96,
  FG_BrightWhite   = 97,
} FG_Color;

typedef enum {
  BG_Black   = 40,
  BG_Red     = 41,
  BG_Green   = 42,
  BG_Yellow  = 43,
  BG_Blue    = 44,
  BG_Magenta = 45,
  BG_Cyan    = 46,
  BG_White   = 47,
  BG_Default = 49,

  BG_BrightBlack   = 100,
  BG_BrightRed     = 101,
  BG_BrightGreen   = 102,
  BG_BrightYellow  = 103,
  BG_BrightBlue    = 104,
  BG_BrightMagenta = 105,
  BG_BrightCyan    = 106,
  BG_BrightWhite   = 107,
} BG_Color;

typedef struct renderTextOpts {
  bool     bold;
  bool     italic;
  bool     underline;
  FG_Color fgColor;
  BG_Color bgColor;
} renderTextOpts;

cstring vinterpol (const cstring message, va_list args);

cstring interpol (const cstring str, ...);

cstring renderstr (const cstring text, renderTextOpts opts);

void printr (const cstring text, renderTextOpts opts);

char* substr (cstring src, uint being, uint end);
char* subnstr (cstring src, uint being, uint count);
bool streq (cstring fStr, cstring sStr);
