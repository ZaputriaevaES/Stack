#include "errorFunctions.h"

extern FILE * logFile; // = Ctor(); Dtor();

int  calculate_hash (void * obj, size_t size)
{
    int hash = 0;
    unsigned char * point = (unsigned char *) obj;

    while (size--)
    {
        hash += ((point[size] << 5) + point[size]);
    }
    return hash;
}

void calculate_stack_hash (stack * stackN)
{
    assert (stackN);

    stackN->hashStack = 0;

    stackN->hashData = calculate_hash (stackN->data - sizeof (leftDataCanary) , sizeof (leftDataCanary) +
                                                                             stackN->capacity*sizeof(stkElem) +
                                                                             sizeof (rightDataCanary));

    stackN->hashStack = calculate_hash (&stackN->leftStackCanary, sizeof (stackN));

    stackN->hashStack += calculate_hash ((char *) &stackN->hashStack, sizeof(stackN->hashStack));
}

int update_stack_hash (stack * stackN)
{
    assert (stackN);

    int stack_hash = calculate_hash (&stackN->leftStackCanary, sizeof (stackN));
    stack_hash    += calculate_hash (stackN->data - sizeof (leftDataCanary) , sizeof (leftDataCanary) +
                                                                               stackN->capacity*sizeof(stkElem) +
                                                                               sizeof (rightDataCanary));


    stack_hash += calculate_hash ((char *) &stack_hash, sizeof(stack_hash));

    return stack_hash;
}

int update_data_hash (stack * stackN)
{
    assert (stackN);

    int data_hash   = calculate_hash (stackN->data - sizeof (leftDataCanary) , sizeof (leftDataCanary) +
                                                                               stackN->capacity*sizeof(stkElem) +
                                                                               sizeof (rightDataCanary));


    return data_hash;
}

bool check_stack_hash (stack * stackN)
{
    assert (stackN);

    if (stackN->hashStack == update_stack_hash (stackN))
        return 0;
    if (stackN->hashData == update_data_hash (stackN))
        return 0;

    return 1;
}


void stackDump_ (stack * stackN, const char * nameCallFunk, int callLine, const char * nameCallFile)
{
    assert(stackN != NULL);

    const char * error = "OK";

    if (stack_ok (stackN) != 0)
        error  = "ERROR";
    /*
    #if MODE == CANARY || MODE == CANARY_HASH
    if(stackN->leftStackCanary  != leftStkCanary)   stackN->errorMask += LEFT_STK_CANARY_ERROR;
    if(stackN->rightStackCanary != rightStkCanary)  stackN->errorMask += RIGHT_STK_CANARY_ERROR;
    if(((canaryType *)stackN->data)[-1]   != leftDataCanary)  stackN->errorMask += LEFT_DATA_CANARY_ERROR;
    if(((canaryType *)stackN->data)[(stackN->capacity) * sizeof(stkElem) / sizeof(canaryType)]
                                                != rightDataCanary) stackN->errorMask += RIGHT_DATA_CANARY_ERROR;
    #endif

    stackN->warInfo.nameCallFunk = nameCallFunk;
    stackN->warInfo.callLine     = callLine;
    stackN->warInfo.nameCallFile = nameCallFile;

    #if MODE == HASH || MODE == CANARY_HASH
    //if(check_stack_hash (stackN) != 0) printf("NOOOOOOOOO\nj");

    if (stackN->hashStack == update_stack_hash (stackN)) stackN->errorMask += HASH_STACK_ERROR;
    if (stackN->hashData  == update_data_hash (stackN))  stackN->errorMask += HASH_DATA_ERROR;
    #endif

    if (stackN == NULL)       stackN->errorMask += STRUCT_POINTER_NOT_FOUND;

    if (stackN->data == NULL) stackN->errorMask += DATA_POINTER_NOT_FOUND;
    */


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

maskType stack_ok (stack *stackN)
{
    maskType error = 0;

    if (stackN == NULL)
    {
        error = error | (1 << STRUCT_POINTER_NOT_FOUND);
        return error;
    }

    if  ((int) stackТ->size > (int) stackТ->capacity)
        error = error | (1 << STACK_OVERFLOW);

    if (stackТ->data == NULL)
        error = error | (1 << DATA_POINTER_NOT_FOUND);
    #ifdef CANARYCHECK
    else
        error = error | (data_canary_ok (stackN) << LEFT_DATA_CANARY_ERROR);
    #endif

    if (((int) stackN->capacity < 0))
        error = error | (1 << STACK_UNDERFLOW);

    #ifdef HASHCHECK
    if (!error )error = error | (check_stack_hash (stackN) << HASH_STACK_ERROR);
    #endif

    #ifdef CANARYCHECK
    error = error | (canary_ok (stackN) << LEFT_STK_CANARY_ERROR);
    #endif

    return error;
}

unsigned short canary_ok (stack *stackN)
{
    assert (stackN);

    unsigned short error = 0;

    if (stackN->left_canary != leftStkCanary)
        error = error | 1;
    else if (stackN->right_canary != rightStkCanary)
        error = error | 2;

    return error;

}

unsigned short data_canary_ok (stack *stackN)
{
    assert (stackN);

    unsigned short error = 0;

    if (((canaryType *)stackN->data)[-1] != leftDataCanary)
        error = error | 1;
    if (((canaryType *)stackN->data)[stackN->capacity*sizeof (stkElem)/sizeof (canaryType)] != rightDataCanary)
        error = error | 2;
    return error;

}

void errorDecod (maskType sumError)
{
    maskType error = sumError;

    for (size_t i = 0; i<= sizeof(int)*8; i++)
    {
        if(((error >> i) & 1) == 1)
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
	case 9:
		fprintf(logFile, "hash stack error,  ");
		break;
	case 10:
		fprintf(logFile, "hash data error,  ");
		break;
	default:
		fprintf(logFile, "no such error exists  ");
	}
}
