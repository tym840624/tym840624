#include "calc.h"
#include <stdio.h>
#include <stdlib.h>


int main()
{
    int a = 10;
    int b = 20;
    printf("%d+%d=%d\n", a, b, add(a,b));
    
    return 0;
}