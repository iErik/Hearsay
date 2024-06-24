#pragma once

#include <stdlib.h>
#include <stdio.h>

#define false 0
#define true 1

#define checkNullPtr(var) if (var == NULL) return NULL;

#define postMalloc(var) if (var == NULL) return NULL;

#define make(type) (type *) malloc(sizeof(type))
#define cmake(type) (type *) calloc(1, sizeof(type))

typedef unsigned char bool;
typedef unsigned int uint;

typedef char* cstring;
typedef FILE* stream;

