#pragma once

#include <stddef.h>

#include "common.h"


typedef struct array array;


array* mkArray ();

void   arrFree (array* list);

void*  arrPush (array* list, void* data);

void*  arrLast (array* list);

size_t arrCap (array* list);

size_t arrLen (array* list);

void*  arrGet  (array* list, uint offset);

void*  arrGrow (array* list, size_t minSize);

void*  arrExpand (array* list);

void*  arrNextItem (array* list);

void   arrResetIter (array* list);
