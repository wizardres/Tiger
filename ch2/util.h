#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef char *string;
string String(char *);
void *checked_malloc(int);
typedef struct U_boolList_ *U_boolList;
struct U_boolList_ { bool head; U_boolList tail; };
U_boolList U_BoolList(bool head,U_boolList tail);
#endif