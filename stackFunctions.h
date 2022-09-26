#ifndef STACKFUNCTIONS_H_INCLUDED
#define STACKFUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>

typedef int stkElem;

typedef struct stack
{
    stkElem * data;
    size_t size;
    size_t capacity;
    unsigned long long errorMask;
    int line;
    const char * nameFunk;

};

enum errors
{
    STRUCT_POINTER_NOT_FOUND = 1,
    DATA_POINTER_NOT_FOUND = 2,
    STACK_OVERFLOW = 4,
    STACK_UNDERFLOW = 8,
    OUT_OF_MEMORY = 16,

};


int stackCtor   (stack * stackN, size_t sizeStackN);

int stackPush   (stack * stackN, size_t value);

int stackPop    (stack * stackN, int * value);

int stackResize (stack * stackN);

int stackDump   (stack * stackN);

int errorDecod  (stack * stackN);

int errorOutput (int i);

int stackDtor   (stack * stackN);

#endif // STACKFUNCTIONS_H_INCLUDED
