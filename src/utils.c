#include <stdlib.h>
#include <string.h>

#include "utils.h"

// Initial size of 1000 bytes (1kb)
#define DYNLIST_INITIAL_CAP 1000

dynlist* mkDynlist () {
  dynlist* list = (dynlist*) malloc(sizeof(dynlist));
  checkNullPtr(list)

  list->data = (void *) calloc(
      DYNLIST_INITIAL_CAP, sizeof(char));
  list->capacity = DYNLIST_INITIAL_CAP;
  list->length= 0;

  return list;
}

void dynPush (dynlist* list, void* data) {
  if (list->capacity < list->capacity + sizeof(data))
    dynGrow(list, list->capacity * 2);

  list->length += 1;
}

void* dynGet(dynlist* list, uint index) {
  if (index > list->length) return NULL;

  return (void *) (list->data + index);
}

void dynGrow (dynlist* list, size_t size) {

}

void dynFree (dynlist* list) {
  free(list->data);
  free(list);
}


char* substr (cstring src, uint begin, uint end) {
  if (strlen(src) < end) return 0;

  uint length = (end - begin) + 1;
  char* str = (char*) malloc(length * sizeof(char));
  checkNullPtr(str)

  memset(str, '\0', length);

  for (int i = begin; i < end; i++) {
    if (*(src + i) == '\0') break;

    *(str + (i - begin)) = *(src + i);
  }

  return str;
}

char* subnstr (cstring src, uint begin, uint count) {
  if (strlen(src) < (begin + count)) return 0;

  uint length = count + 1;
  char* str = (char*) malloc(length * sizeof(char));
  checkNullPtr(str)

  memset(str, '\0', length);

  for (int i = begin; i < (begin + count); i++) {
    if (*(src + i) == '\0') break;

    *(str + (i - begin)) = *(src + i);
  }

  return str;
}


