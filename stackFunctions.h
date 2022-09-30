#ifndef STACKFUNCTIONS_H_INCLUDED
#define STACKFUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <malloc.h>

#define RELIZE 1
#define CANARY 2
#define HASH 3
#define CANARY_HASH 4

#define MODE CANARY_HASH

#if MODE == CANARY || MODE == CANARY_HASH
#define leftStkCanary   (unsigned long long)((unsigned long long)stackN | 0xDEADBABE)
#define rightStkCanary  (unsigned long long)((unsigned long long)stackN | 0xBADDCAFE)
#define leftDataCanary  (unsigned long long)0xDEADBEEF
#define rightDataCanary (unsigned long long)0xBAADF00D
#endif

#define elemPoison      45629746
#define elemPoisonClean 69873621

#define stackCtor(stk, size) stackCtor_((stk), (size), #stk+1, __FUNCTION__, __LINE__, __FILE__);

#define stackDump(stk) stackDump_ ((stk), __FUNCTION__, __LINE__, __FILE__);

typedef int stkElem;

typedef struct warrningInfo
{
    const char * nameStack;

    int          creatLine;
    const char * nameCreatFunk;
    const char * nameCreatFile;

    int          callLine;
    const char * nameCallFunk;
    const char * nameCallFile;
};

typedef struct stack
{
    #if MODE == CANARY || MODE == CANARY_HASH
    unsigned long long leftStackCanary;
    #endif

    stkElem * data;
    size_t    size;
    size_t    capacity;

    warrningInfo warInfo;

    unsigned long long errorMask;

    #if MODE == HASH || MODE == CANARY_HASH
    unsigned long long hashData;
    #endif

    #if MODE == CANARY || MODE == CANARY_HASH
    unsigned long long rightStackCanary;
    #endif

    #if MODE == HASH || MODE == CANARY_HASH
    unsigned long long hashStack;
    #endif
};

enum errors
{
    STRUCT_POINTER_NOT_FOUND = 1,
    DATA_POINTER_NOT_FOUND   = 2,
    STACK_OVERFLOW           = 4,
    STACK_UNDERFLOW          = 8,
    OUT_OF_MEMORY            = 16,
    LEFT_STK_CANARY_ERROR    = 32,
    RIGHT_STK_CANARY_ERROR   = 64,
    LEFT_DATA_CANARY_ERROR   = 128,
    RIGHT_DATA_CANARY_ERROR  = 256,

};


void stackCtor_  (stack * stackN, size_t sizeStackN, const char * nameStack,
                 const char * namecreatFunk, int creatLine, const char * nameCreatFile);

void stackPush   (stack * stackN, size_t value);

void stackPop    (stack * stackN, int * value);

void stackResize (stack * stackN);

void stackDump_  (stack * stackN, const char * nameCallFunk, int callLine, const char * nameCallFile);

void errorDecod  (unsigned long long sumError);

void errorOutput (int i);

void stackDtor   (stack * stackN);

#endif // STACKFUNCTIONS_H_INCLUDED
