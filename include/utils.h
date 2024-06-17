#pragma once

#include <stddef.h>
#include "common.h"

#define m_defDynlist(NAME, TYPE) typedef struct NAME { \
  TYPE data;                                           \
  size_t capacity;                                     \
  size_t size;                                         \
} NAME;

typedef struct dynlist {
  void*  data;
  size_t capacity;
  size_t length;
} dynlist;

typedef struct linklist {
  struct linklist* next;
  struct linklist* prev;
} linklist;


char* substr (cstring src, uint being, uint end);
char* subnstr (cstring src, uint being, uint count);

dynlist* mkDynlist ();

void dynPush (dynlist* list, void* data);

void* dynGet (dynlist* list, uint index);

void dynGrow (dynlist* list, size_t size);

void dynFree (dynlist* list);
