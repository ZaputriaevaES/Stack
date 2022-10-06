#ifndef STACKFUNCTIONS_H_INCLUDED
#define STACKFUNCTIONS_H_INCLUDED

#include "utilities.h"

int  calculate_hash (void * obj, size_t size);
void calculate_stack_hash (stack * stackN);
int update_stack_hash (stack *stackN);
int update_data_hash (stack *stackN);
bool check_stack_hash (stack *stackN);
maskType stack_ok (stack *stackN);
unsigned short canary_ok (stack *stackN);
unsigned short data_canary_ok (stack *stackN);

void stackCtor_   (stack * stackN, size_t sizeStackN, const char * nameStack,
                  const char * namecreatFunk, int creatLine, const char * nameCreatFile);

void stackPush    (stack * stackN, size_t value);

void stackPop     (stack * stackN, int * value);

void stackResize  (stack * stackN);

void stackDump_   (stack * stackN, const char * nameCallFunk, int callLine, const char * nameCallFile);

void stackDtor    (stack * stackN);

void * recalloc   (void * memoryPtr, size_t numElem, size_t sizeElem);

hashType hashFunc (void * pointObject, size_t sizeObject);

#endif // STACKFUNCTIONS_H_INCLUDED
