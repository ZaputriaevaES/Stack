#include "stackFunctions.h"
#include <math.h>

#define ASSERT(stk) stk->line = __LINE__; stk->nameFunk = __FUNCTION__; stackDump (stk);

extern FILE * logFile = fopen("logFile.txt", "w+");


int stackCtor (stack * stackN, size_t sizeStackN)
{
    assert(stackN != NULL);

    stackN->capacity = 0;
    stackN->size = 0;

    stackN->data = (stkElem *)calloc(sizeStackN, sizeof(stkElem));

    if (stackN->data == NULL)
    {
        free(stackN->data);
        stackN->errorMask += OUT_OF_MEMORY;
    }

    stackN->capacity = sizeStackN;

    ASSERT(stackN);

    return 0;
}

int stackPush (stack * stackN, size_t value)
{
    ASSERT(stackN);

    if (stackN->size >= stackN->capacity) stackResize(stackN);

    stackN->data[stackN->size++] = value;

    ASSERT(stackN);

    return 0;
}

int stackPop (stack * stackN, int * value)
{
    ASSERT(stackN);

    assert(value != NULL);

    if (stackN->size == 0 || stackN->capacity == 0) stackN->errorMask += STACK_UNDERFLOW;

    if (stackN->size <= stackN->capacity/4 && stackN->size >= 16) stackResize(stackN);

    stackN->size--;

    *value = stackN->data[stackN->size];

    ASSERT(stackN);

    return 0;
}

int stackResize (stack * stackN)
{
    ASSERT(stackN);

    if (stackN->size >= stackN->capacity) stackN->capacity *= 2;

    else if (stackN->size <= stackN->capacity/4 && stackN->size >= 16) stackN->capacity /= 4;

    stackN->data = (stkElem *)realloc(stackN->data, (stackN->capacity) * sizeof(stkElem));

    if (stackN->data == NULL) stackN->errorMask += OUT_OF_MEMORY;

    ASSERT(stackN);

    return 0;
}

int stackDump (stack * stackN)
{
    ///if (stackN == NULL) fprintf(logFile, "stackN == NULL");
    //else fprintf(logFile, "stackN is ok\n");

    //if (stackN->data == NULL) fprintf(logFile, "stackN->data == NULL");
    //else fprintf(logFile, "stackN->data is ok\n");

    if(stackN->errorMask == 0)
    {
        fprintf(logFile, "stack[%p] (ok) \"stk1\" at %s(%d)\n", &stackN, stackN->nameFunk, stackN->line);

        //for (int i = 0; i < stackN->size; i++) fprintf(logFile, "data[%d] = %d\n", i, stackN->data[i]);
    }
    else errorDecod(stackN);

    return 0;
}

int errorDecod (stack * stackN)
{
    unsigned long long sumError = stackN->errorMask;

    for(int i = 0; sumError > 0; i++)
    {
        unsigned long long a = pow(2, i);

        if(a == sumError)
        {
            errorOutput(i);
            fprintf(logFile, "in stack[%p] \"stk1\" at %s(%d)\n", &stackN, stackN->nameFunk, stackN->line);
        }

        if(a >= sumError) return 1;

        if((sumError - a)%(a*2) == 0)
        {
            errorOutput(i);
            sumError -= a;
        }
    }

    return 1;
}

int errorOutput(int i)
{

	switch (i) {
	case 0:
		fprintf(logFile, "structure not found  ");
		break;
	case 1:
		fprintf(logFile, "date pointer not found  ");
		break;
	case 2:
		fprintf(logFile, "stack overflow  ");
		break;
	case 3:
		fprintf(logFile, "stack underflow  ");
		break;
	case 4:
		fprintf(logFile, "failed to allocate memory  ");
		break;
	case 5:
		fprintf(logFile, "-  ");
		break;
	default:
		fprintf(logFile, "no such error exists  ");
	}

    return 1;
}

int stackDtor (stack * stackN)
{
    assert(stackN != NULL);

    free(stackN->data);

    stackN->data = NULL;

    stackN->size = -1;

    stackN->capacity = -1;

    stackN = NULL;

    return 0;
}
