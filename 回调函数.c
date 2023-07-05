#include<stdio.h>
#include<stdlib.h>

int Max(int a, int b)
{
    return a > b ? a : b;
}

int Min(int a, int b)
{
     return a < b ? a : b;
}

int find_data(int a, int b, int(*pfun)(int, int))
{
    return pfun(a,b);
}

int main()
{
    int a = 10;
    int b = 20;

    int iMax = find_data(a, b, Max);
    printf("Max = %d\n", iMax);

    int iMin = find_data(a, b, Min);
    printf("Min = %d\n", iMin);

    return 0;
}