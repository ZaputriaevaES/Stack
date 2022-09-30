#include "stackFunctions.h"
#include <math.h>

extern FILE * logFile = fopen("logFile.txt", "w+");

#if MODE == HASH || MODE == CANARY_HASH
static unsigned long long hashFunc (void * pointObject, size_t sizeObject)
{
    assert(pointObject != NULL);

    unsigned long long hashValue = 0;

    unsigned char * point = (unsigned char *) pointObject;

    for(size_t i = 0; i < sizeObject; i += 10)
    {
        hashValue += point[sizeObject];
    }

    return hashValue;
}
#endif

#if MODE == CANARY || MODE == CANARY_HASH
static stkElem  * creatStackData(stack * stackN)
{
    assert(stackN != NULL);

    unsigned long long * stackData = (unsigned long long *)calloc(1, (sizeof(leftDataCanary) + stackN->capacity * sizeof(stkElem) + sizeof(rightDataCanary)));

    if (stackData == NULL)
    {
        free(stackN->data);
        stackN->errorMask += OUT_OF_MEMORY;
    }

    stackData[0] = leftDataCanary;
    stackData++;
    stackData[(stackN->capacity) * sizeof(stkElem) / sizeof(unsigned long long)] = rightDataCanary;

    return (stkElem  *)stackData;
}

static stkElem  * recreatStackData(stack * stackN)
{
    assert(stackN != NULL);

    unsigned long long * stackData = (unsigned long long *)stackN->data;

    stackData--;

    stackData = (unsigned long long *) realloc(stackData, (sizeof(leftDataCanary) + stackN->capacity * sizeof(stkElem) + sizeof(rightDataCanary)));

    if (stackData == NULL)
    {
        free(stackData);
        stackN->errorMask += OUT_OF_MEMORY;
    }

    stackData[0] = leftDataCanary;
    stackData++;
    stackData[(stackN->capacity) * sizeof(stkElem) / sizeof(unsigned long long)] = rightDataCanary;

    return (stkElem  *)stackData;
}
#endif

void stackCtor_ (stack * stackN, size_t sizeStackN, const char * nameStack, const char * nameCreatFunk, int creatLine, const char * nameCreatFile)
{
    assert(stackN != NULL);

    #if MODE == CANARY || MODE == CANARY_HASH
    stackN->leftStackCanary  = leftStkCanary;
    stackN->rightStackCanary = rightStkCanary;
    #endif

    stackN->warInfo.nameStack     = nameStack;
    stackN->warInfo.nameCreatFunk = nameCreatFunk;
    stackN->warInfo.creatLine     = creatLine;
    stackN->warInfo.nameCreatFile = nameCreatFile;

    stackN->capacity = sizeStackN;
    stackN->size     = 0;

    #if MODE == CANARY || MODE == CANARY_HASH
    stackN->data = creatStackData(stackN);
    #endif

    #if MODE == RELIZE || MODE == HASH
    stackN->data = (stkElem *)calloc(sizeStackN, sizeof(stkElem));

    if (stackN->data == NULL)
    {
        free(stackN->data);
        stackN->errorMask += OUT_OF_MEMORY;
    }
    #endif

    for (int i = 0; i < sizeStackN; i++)
    {
        stackN->data[i] = elemPoison;
    }

    #if MODE == HASH || MODE == CANARY_HASH
    stackN->hashData  = hashFunc(stackN->data, sizeof(stkElem)*stackN->capacity);
    stackN->hashStack = hashFunc(stackN, sizeof(*stackN) - sizeof(unsigned long long));
    #endif

    //printf("%x\n", ((unsigned long long *)stackN->data)[-1]);
    //printf("%x\n", ((unsigned long long *)stackN->data)[(stackN->capacity) * sizeof(stkElem) / sizeof(unsigned long long)] );

    stackDump(stackN);
}

void stackPush (stack * stackN, size_t value)
{
    assert(stackN != NULL);

    stackDump(stackN);

    if (stackN->size >= stackN->capacity) stackResize(stackN);

    stackN->data[stackN->size++] = value;

    stackDump(stackN);
}

void stackPop (stack * stackN, int * value)
{
    assert(stackN != NULL);

    assert(value != NULL);

    stackDump(stackN);

    if (stackN->size == 0 || stackN->capacity == 0) stackN->errorMask += STACK_UNDERFLOW;

    if (stackN->size > 0)
    {
        stackN->size--;

        *value = stackN->data[stackN->size];

        stackN->data[stackN->size] = -111;

        if (stackN->size <= stackN->capacity/4 && stackN->size >= 16) stackResize(stackN);
    }

    stackDump(stackN);
}

void stackResize (stack * stackN)
{
    assert(stackN != NULL);

    stackDump(stackN);

    if      (stackN->size >= stackN->capacity)                         stackN->capacity *= 2;

    else if (stackN->size <= stackN->capacity/4 && stackN->size >= 16) stackN->capacity /= 4;

    #if MODE == CANARY || MODE == CANARY_HASH
    stackN->data = recreatStackData(stackN);
    #endif

    #if MODE == RELIZE || MODE == HASH
    stackN->data = (stkElem *)realloc(stackN->data, (stackN->capacity) * sizeof(stkElem));
    #endif

    if (stackN->data == NULL) stackN->errorMask += OUT_OF_MEMORY;

    #if MODE == HASH || MODE == CANARY_HASH
    stackN->hashData  = hashFunc(stackN->data, sizeof(stkElem)*stackN->capacity);
    stackN->hashStack = hashFunc(stackN, sizeof(*stackN) - sizeof(unsigned long long));
    #endif

    for (size_t i = stackN->size; i < stackN->capacity; i++)
        stackN->data[i] = elemPoison;

    stackDump(stackN);
}

void stackDump_ (stack * stackN, const char * nameCallFunk, int callLine, const char * nameCallFile)
{
    assert(stackN != NULL);

    #if MODE == CANARY || MODE == CANARY_HASH
    if(stackN->leftStackCanary  != leftStkCanary)   stackN->errorMask += LEFT_STK_CANARY_ERROR;
    if(stackN->rightStackCanary != rightStkCanary)  stackN->errorMask += RIGHT_STK_CANARY_ERROR;
    if (((unsigned long long *)stackN->data)[-1]   != leftDataCanary)  stackN->errorMask += LEFT_DATA_CANARY_ERROR;
    if(((unsigned long long *)stackN->data)[(stackN->capacity) * sizeof(stkElem) / sizeof(unsigned long long)]
                                                != rightDataCanary) stackN->errorMask += RIGHT_DATA_CANARY_ERROR;
    #endif

    stackN->warInfo.nameCallFunk = nameCallFunk;
    stackN->warInfo.callLine     = callLine;
    stackN->warInfo.nameCallFile = nameCallFile;

    #if MODE == HASH || MODE == CANARY_HASH
    unsigned long long hashValueNew = 0;

    assert(( hashValueNew = hashFunc(stackN->data, sizeof(stkElem)*stackN->capacity) ) == stackN->hashData);
    stackN->hashData = hashValueNew;

    hashValueNew = 0;

    assert(( hashValueNew = hashFunc(stackN, sizeof(*stackN) - sizeof(unsigned long long)) ) == stackN->hashStack);
    stackN->hashStack = hashValueNew;
    #endif

    if (stackN == NULL)       stackN->errorMask += STRUCT_POINTER_NOT_FOUND;

    if (stackN->data == NULL) stackN->errorMask += DATA_POINTER_NOT_FOUND;

    if(stackN->errorMask == 0)
    {
        fprintf(logFile, "stack[%p] (OK) \"%s\" called at %s(%d) in %s, \ncreated at %s(%d) in %s\n",
                              &stackN, stackN->warInfo.nameStack, stackN->warInfo.nameCallFunk,
                              stackN->warInfo.callLine, stackN->warInfo.nameCallFile,
                              stackN->warInfo.nameCreatFunk,
                              stackN->warInfo.creatLine, stackN->warInfo.nameCreatFile);

        for (size_t i = 0; i < stackN->capacity; i++) fprintf(logFile, "data[%d] = %d\n", i, stackN->data[i]);
    }

    else
    {
        errorDecod(stackN->errorMask);

        fprintf(logFile, "in stack[%p] \"%s\" called at %s(%d) in %s, \ncreated at %s(%d) in %s\n",
                              &stackN, stackN->warInfo.nameStack, stackN->warInfo.nameCallFunk,
                              stackN->warInfo.callLine, stackN->warInfo.nameCallFile,
                              stackN->warInfo.nameCreatFunk,
                              stackN->warInfo.creatLine, stackN->warInfo.nameCreatFile);
    }
}

void errorDecod (unsigned long long sumError)
{
    unsigned long long error = sumError;

    for (size_t i = 0; i<= sizeof(int)*8; i++)
    {
        if((error >> i) & 1 == 1)
        {
            errorOutput(i);
        }
        error = sumError;
    }
}

void errorOutput (int i)
{

	switch (i) {
	case 0:
		fprintf(logFile, "structure not found,  ");
		break;
	case 1:
		fprintf(logFile, "date pointer not found,  ");
		break;
	case 2:
		fprintf(logFile, "stack overflow,  ");
		break;
	case 3:
		fprintf(logFile, "stack underflow,  ");
		break;
	case 4:
		fprintf(logFile, "failed to allocate memory,  ");
		break;
	case 5:
		fprintf(logFile, "spoiled left canary in the stack,  ");
		break;
	case 6:
		fprintf(logFile, "spoiled right canary in the stack,  ");
		break;
	case 7:
		fprintf(logFile, "spoiled left canary in the data,  ");
		break;
	case 8:
		fprintf(logFile, "spoiled left canary in the data,  ");
		break;
	default:
		fprintf(logFile, "no such error exists  ");
	}
}

void stackDtor (stack * stackN)
{
    assert(stackN != NULL);

    stackN->size = -1;

    stackN->capacity = -1;

    stackN->warInfo = {};

    free(stackN->data);

    stackN->data = NULL;

    stackN = NULL;
}
