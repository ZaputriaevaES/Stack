#include "stackFunctions.h"

int main()
{
    FILE * logFile = fopen("logFile.txt", "w+"); // log file ctor
    assert(logFile != NULL);

    stack stk1 = {0};

    stackCtor(&stk1, 16);

    size_t i = 0;

    for (i = 0; i < 50; i++)
    {
        stackPush(&stk1, i);
    }

    for (i = 0; i < stk1.size; i++)
    {
        printf("%10d", stk1.data[i]);
    }
    printf("\n %d %d \n", stk1.size, stk1.capacity);

    int value = 0;

    for (i = 0; i < 50; i++)
    {
        stackPop(&stk1, &value);
        printf("%6d-%3d", value, stk1.capacity);
        value = 0;
    }

    stackDtor(&stk1);

    fclose(logFile);

    return 0;
}
