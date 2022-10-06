#ifndef ERRORFUNCTIONS_H_INCLUDED
#define ERRORFUNCTIONS_H_INCLUDED

#include "utilities.h"

//int  calculate_hash (void * obj, size_t size);
//void calculate_stack_hash (stack * stackN);
//int update_stack_hash (stack *stackN);
//int update_data_hash (stack *stackN);
//bool check_stack_hash (stack *stackN);

void * recalloc   (void * memoryPtr, size_t numElem, size_t sizeElem);

hashType hashFunc (void * pointObject, size_t sizeObject);

void stackDump_   (stack * stackN, const char * nameCallFunk, int callLine, const char * nameCallFile);

void errorDecod   (maskType sumError);

void errorOutput  (int i);

#endif // ERRORFUNCTIONS_H_INCLUDED
