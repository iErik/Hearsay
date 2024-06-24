#include <stdlib.h>


// Variable declaration:
// <type> <name> = <value>;
// <type> <name>[<num items>] = <value>;

// 32-bits = 2 bytes
// 2 * 10
int arr[10];   // 0x100

// [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ]

// ASCII Table
// A = 130
char cc = 'c'; // 0x120

// 16 - 32-bit
int    ii = 10;

// 16 / 32-bit
float  ff = 10.3;

// 64-bits
double dd = 10.2;


/*
<return type> <function name> (<parameter list>) {
  ...<function body>
}
*/

//int variable = 0;

// int main () { print("Hi"); int variable = 10; }

// int func () { }

typedef struct Test {
  char* buff;
} Test;

int main () {
  Test* myTest = (Test *) malloc(sizeof(Test));
  if (myTest == NULL) { return 1; }

  myTest->buff = (char *) calloc(10, sizeof(char));
  if (myTest->buff == NULL) { return 1; }

  free(myTest);

  return 0;
}
