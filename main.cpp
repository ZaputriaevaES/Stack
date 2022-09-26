#include "stackFunctions.h"

int main()
{
    FILE * logFile = fopen("logFile.txt", "w+");
    assert(logFile != NULL);

    stack stk1 = {NULL, -1, -1, 0, __LINE__, __FUNCTION__};

    stackCtor(&stk1, 6);

    int i = 0;

    for (i = 0; i < 100; i++)
    {
        stackPush(&stk1, i);
    }

    for (i = 0; i < stk1.size; i++)
    {
        printf("%10d", stk1.data[i]);
    }
    printf("\n %d %d \n", stk1.size, stk1.capacity);

    int value = 0;

    for (i = 0; i < 100; i++)
    {
        stackPop(&stk1, &value);
        printf("%6d-%3d", value, stk1.capacity);
        value = 0;
    }

    stackDtor(&stk1);

    fclose(logFile);

    return 0;
}