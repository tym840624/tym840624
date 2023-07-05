#include<stdio.h>
#include<stdlib.h>

enum {QUIT, ADD, SUB, MUL};

int Add(int a, int b)
{
    return a+b;
}

int Sub(int a, int b)
{
    return a-b;
}

int Mul(int a, int b)
{
    return a*b;
}


//定义一个函数指针数组
/*
    int *ar[3];   //指针数组，数组元素是指针
    int (*ar)[3];  //数组指针，ar指向有3个整形元素的数组

    *func_table[]是数组，存放的指针，指针指向返回值int, 两个整型为参数的函数
*/
int (*func_table[])(int,int) = {NULL, Add, Sub, Mul};

int main()
{
    int select = 1;
    while(select)
    {
        printf("**************************\n");
        printf("*       简易计算器         *\n");
        printf("* [1] Add       [2] Sub  *\n");
        printf("* [3] Mul       [0] Exit *\n");
        printf("**************************\n");
        printf("请选择:>");
        scanf("%d", &select);

        if( select  == QUIT){
            break;
        }

        int opt1, opt2, result;
        printf("请输入两个数:>");
        scanf("%d %d", &opt1, &opt2);

        /*
        switch (select)
        {
        case ADD:
            result = Add(opt1, opt2);
            break;
        case SUB:
            result = Sub(opt1, opt2);
            break;
        case MUL:
            result = Mul(opt1, opt2);
            break;
        default:
            break;
        }
        */

        result = func_table[select](opt1, opt2);        //以后加除法等，就不需要加switch了
        printf("res = %d\n", result);
    }
    return 0;
}