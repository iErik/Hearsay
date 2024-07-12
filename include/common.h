#pragma once

#include <stdlib.h>
#include <stdio.h>

#define false 0
#define true 1

#define checkNullPtr(var) if (var == NULL) return NULL;

#define postMalloc(var) if (var == NULL) return NULL;

#define make(T) (T *) malloc(sizeof(T))
#define cmake(T) (T *) calloc(1, sizeof(T))
#define cmaken(C, T) (T *) calloc(C, sizeof(T))

typedef unsigned char bool;
typedef unsigned int uint;

typedef char* cstring;
typedef FILE* stream;

