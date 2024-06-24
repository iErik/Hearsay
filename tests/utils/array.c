#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "tests/array.h"
#include "tests/test-utils.h"

#include "utils/array.h"

bool streq(cstring strOne, cstring strTwo) {
  return strcmp(strOne, strTwo) == 0;
}

void TestArrayIterator () { }

void TestArrayPush () {
  testing("Array Push");

  array* myArray = mkArray();

  expect(arrLen(myArray) == 0,
    "Size mismatch\n"
    "Expected: %i\n"
    "Received: %i\b",
    0, arrLen(myArray));

  cstring first  = "This is";
  cstring second = "one really cool";
  cstring third  = "Test!";
  cstring fourth = "banona";

  void* offset = arrPush(myArray, first);
  expect(streq(offset, first),
    "String mismatch!\n"
    "Expected: %s\n"
    "Received: %s\n",
    first, offset);
  expect(streq(arrGet(myArray, 0), first),
    "String mismatch!\n"
    "Expected: %s\n"
    "Received: %s\n",
    first, offset);
  expect(arrLen(myArray) == 1,
    "Size mismatch\n"
    "Expected: %i\n"
    "Received: %i\b",
    1, arrLen(myArray));

  offset = arrPush(myArray, second);
  expect(streq(offset, second),
    "String mismatch!\n"
    "Expected: %s\n"
    "Received: %s\n",
    second, offset);
  expect(streq(arrGet(myArray, 1), second),
    "String mismatch!\n"
    "Expected: %s\n"
    "Received: %s\n",
    second, offset);
  expect(arrLen(myArray) == 2,
    "Size mismatch\n"
    "Expected: %i\n"
    "Received: %i\n",
    2, arrLen(myArray));

  offset = arrPush(myArray, third);
  expect(streq(offset, third),
    "String mismatch!\n"
    "Expected: %s\n"
    "Received: %s\n",
    third, offset);
  expect(streq(arrGet(myArray, 2), third),
    "String mismatch!\n"
    "Expected: %s\n"
    "Received: %s\n",
    third, offset);
  expect(arrLen(myArray) == 3,
    "Size mismatch\n"
    "Expected: %i\n"
    "Received: %i\n",
    3, arrLen(myArray));

  offset = arrPush(myArray, fourth);
  expect(streq(offset, fourth),
    "String mismatch!\n"
    "Expected: %s\n"
    "Received: %s\n",
    fourth, offset);
  expect(streq(arrGet(myArray, 3), fourth),
    "String mismatch!\n"
    "Expected: %s\n"
    "Received: %s\n",
    fourth, offset);
  expect(arrLen(myArray) == 4,
    "Size mismatch\n"
    "Expected: %i\n"
    "Received: %i\n",
    4, arrLen(myArray));

  printf("Offset: %p\n", offset);
}

void TestArrayCreation () {

}

void TestArray () {
  TestArrayPush();
}
