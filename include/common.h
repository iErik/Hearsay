#pragma once

#include <stdio.h>

#define false 0
#define true 1

#define checkNullPtr(var) if (var == NULL) return NULL;

#define postMalloc(var) if (var == NULL) return NULL;

typedef unsigned char bool;
typedef unsigned int uint;

typedef char* cstring;
typedef FILE* stream;

