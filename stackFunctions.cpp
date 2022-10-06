#include "stackFunctions.h"

extern FILE * logFile = fopen("logFile.txt", "w+");



#if MODE == CANARY || MODE == CANARY_HASH
static stkElem  * creatStackData(stack * stackN)
{
    assert(stackN != NULL);

    canaryType * stackData = (canaryType *)calloc(1, (2*sizeof(canaryType) + stackN->capacity * sizeof(stkElem)));

    if (stackData == NULL)
    {
        free(stackN->data);
        stackN->errorMask += OUT_OF_MEMORY;
    }

    stackData[0] = leftDataCanary;
    stackData++;
    stackData[(stackN->capacity) * sizeof(stkElem) / sizeof(canaryType)] = rightDataCanary;  //char

    return (stkElem  *)stackData;
}

static stkElem  * recreatStackData(stack * stackN)
{
    assert(stackN != NULL);

    unsigned long long * stackData = (unsigned long long *)stackN->data;

    stackData--;

    stackData = (unsigned long long *) recalloc(stackData, (2*sizeof(canaryType) + stackN->capacity * sizeof(stkElem)), 1);

    if (stackData == NULL)
    {
        free(stackData);
        stackN->errorMask += OUT_OF_MEMORY;
    }

    stackData[0] = leftDataCanary;
    stackData++;
    stackData[(stackN->capacity) * sizeof(stkElem) / sizeof(canaryType)] = rightDataCanary;

    return (stkElem  *)stackData;
}
#endif

void * recalloc(void * memoryPtr, size_t numElem, size_t sizeElem)
{
    assert(memoryPtr != NULL);

    size_t allMemorySize = numElem * sizeElem;

    size_t busyMemorySize = _msize(memoryPtr);

    memoryPtr = realloc(memoryPtr, allMemorySize);

    if(memoryPtr == NULL) return NULL;

    if (allMemorySize > busyMemorySize)
        memset((char *)memoryPtr + busyMemorySize, 0, allMemorySize - busyMemorySize);

    return memoryPtr;
}

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

    if(sizeStackN >= 16) stackN->capacity = sizeStackN; //const
    else stackN->capacity = 16;

    stackN->size = 0;

    #if MODE == CANARY || MODE == CANARY_HASH
    stackN->data = creatStackData(stackN);     //general funk
    #endif

    #if MODE == RELIZE || MODE == HASH
    stackN->data = (stkElem *)calloc(stackN->capacity, sizeof(stkElem));

    if (stackN->data == NULL)
    {
        free(stackN->data);
        stackN->errorMask += OUT_OF_MEMORY;
    }
    #endif

    for (size_t i = 0; i < stackN->capacity; i++)
    {
        stackN->data[i] = elemPoison;  //funk
    }

    #if MODE == HASH || MODE == CANARY_HASH
    calculate_stack_hash (stackN);
    #endif

    stackDump(stackN);
}

void stackPush (stack * stackN, size_t value)
{
    assert(stackN != NULL);

    stackDump(stackN);

    if (stackN->size >= stackN->capacity) stackResize(stackN);

    stackN->data[stackN->size++] = value;

    #if MODE == HASH || MODE == CANARY_HASH
        calculate_stack_hash (stackN);
    #endif

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

        stackN->data[stackN->size] = elemPoisonClean;

        if (stackN->size <= stackN->capacity/4 && stackN->size >= 16) stackResize(stackN);
    }

    #if MODE == HASH || MODE == CANARY_HASH
        calculate_stack_hash (stackN);
    #endif

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
    stackN->data = (stkElem *)recalloc(stackN->data, (stackN->capacity) * sizeof(stkElem), 1);
    #endif

    if (stackN->data == NULL)
    {
        free(stackN->data);
        stackN->errorMask += OUT_OF_MEMORY;
    }

    for (size_t i = stackN->size; i < stackN->capacity; i++)
        stackN->data[i] = elemPoison;

    stackDump(stackN);
}

void stackDtor (stack * stackN)
{
    assert(stackN != NULL);

    stackN->size = -1;

    stackN->capacity = -1;

    stackN->warInfo = {0,0,0,0,0,0,0};

    free(stackN->data);

    stackN->data = NULL;

    stackN = NULL;
}
