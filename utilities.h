#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

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
#define leftStkCanary   (canaryType)((canaryType)stackN | 0xDEADBABE)
#define rightStkCanary  (canaryType)((canaryType)stackN | 0xBADDCAFE)
#define leftDataCanary  (canaryType)0xDEADBEEF
#define rightDataCanary (canaryType)0xBAADF00D
#endif

#define elemPoison      45629746
#define elemPoisonClean 69873621

#define stackCtor(stk, size) stackCtor_((stk), (size), #stk+1, __FUNCTION__, __LINE__, __FILE__);

#define stackDump(stk) stackDump_ ((stk), __FUNCTION__, __LINE__, __FILE__);

typedef int stkElem;
typedef unsigned long long canaryType;
typedef int hashType;
typedef unsigned long long maskType;

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
    canaryType leftStackCanary;
    #endif

    stkElem * data;
    size_t    size;
    size_t    capacity;

    warrningInfo warInfo; // mode release-release

    maskType errorMask;

    #if MODE == HASH || MODE == CANARY_HASH
    hashType hashData;
    #endif

    #if MODE == CANARY || MODE == CANARY_HASH
    canaryType rightStackCanary;
    #endif

    #if MODE == HASH || MODE == CANARY_HASH
    hashType hashStack;
    #endif
};

enum errors
{
    STRUCT_POINTER_NOT_FOUND = 0, // 1<<STRUCT_POINTER_NOT_FOUND
    DATA_POINTER_NOT_FOUND   = 1,
    STACK_OVERFLOW           = 2,
    STACK_UNDERFLOW          = 3,
    OUT_OF_MEMORY            = 4,
    LEFT_STK_CANARY_ERROR    = 5,
    RIGHT_STK_CANARY_ERROR   = 6,
    LEFT_DATA_CANARY_ERROR   = 7,
    RIGHT_DATA_CANARY_ERROR  = 8,
    HASH_STACK_ERROR         = 9,
    HASH_DATA_ERROR          = 10

};

#endif // UTILITIES_H_INCLUDED
