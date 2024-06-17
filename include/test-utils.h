#pragma once

#include <stdarg.h>

void vfail (const char* message, va_list args);

void fail (const char* emssage, ...);

void expect (int condition, const char* errorMessage, ...);
