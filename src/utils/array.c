#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils/array.h"

// Initial size of 100 (100 * sizeof(void *) Bytes)
#define INITIAL_CAP 128


typedef struct array {
  void** data;
  void** _offsetPtr;
  void** _iteratorPtr;

  size_t _size;
  size_t _capacity;
} array;


array* mkArray () {
  array* list = (array*) malloc(sizeof(array));
  checkNullPtr(list);

  list->data = (void *) calloc(INITIAL_CAP, sizeof(void *));
  checkNullPtr(list->data);

  list->_offsetPtr = list->data;
  list->_iteratorPtr = list->data;
  list->_capacity = INITIAL_CAP;
  list->_size = 0;

  return list;
}

size_t arrLen (array* list) {
  return list->_size;
}

void* arrLast (array* list) {
  return *list->_offsetPtr;
}

size_t arrCap (array* list) {
  return list->_capacity;
}

void* arrPush (array* list, void* data) {
  size_t cap = list->_capacity;
  size_t mincap = cap + sizeof(void *);

  if (cap < mincap) if (arrExpand(list) == NULL)
    return NULL;

  void** offset = (list->data + list->_size);

  *offset = data;
  list->_offsetPtr = offset;
  list->_size++;

  return *offset;
}

void* arrGet (array* list, uint offset) {
  if (offset > arrLen(list)) return NULL;
  return *(list->data + offset);
}

void* arrExpand (array* list) {
  size_t newCap = arrCap(list) * 2;
  void* dataPtr = realloc(list->data, newCap);
  checkNullPtr(dataPtr);

  list->data = dataPtr;
  list->_capacity = newCap;

  return list;
}

void* arrGrow (array* list, size_t size) {
  size_t newSize = arrCap(list) + size;
  void* dataPtr = realloc(list->data, newSize);
  checkNullPtr(dataPtr);

  list->data = dataPtr;
  return list;
}

void* arrNextItem (array* list) {
  if (list->_iteratorPtr == NULL) return NULL;

  void** item = list->_iteratorPtr;

  if (item == list->_offsetPtr)
    list->_iteratorPtr = NULL;
  else
    list->_iteratorPtr++;

  return item;
}

void arrResetIter (array* list) {
  list->_iteratorPtr = list->data;
}

void arrFree (array* list) {
  free(list->data);
  free(list);
}
