#include <stdio.h>
#define ADD(x, y) ((x) + (y))
int  main()
{
    int a = 5, b = 3;
    int sum = ADD(a, b);
    printf("The sum of %d and %d is: %d\n", a, b, sum);
    return 0;
    }