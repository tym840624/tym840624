#include<stdio.h>
#include<stdlib.h>

char* Conver_16(unsigned long value)
{
    static char buffer[sizeof(unsigned long) + 1];
    for(int i=sizeof(unsigned long) ; i>=0; --i)
    {
        int mod = value % 16;
        if(mod >= 10)
        {
            buffer[i] = mod - 10 + 'A';      //10A 11B 12C 13D 14E 15F
        }
        else
        {
            buffer[i] = mod + '0';      //转为字符类型的数字
        }
        value /= 16;
    }
    return buffer;
}

int main()
{
    unsigned long value = 65533;
    
    char* res = Conver_16(value);
    printf("0X%s\n", res);
    return 0;
}


/*
int main()
{
    "xyz" + 1;
    printf("%s\n" , "xyz"+1);       //yz        "xyz" 代表的是存储在静态区的字符串首地址 "xyz"在内存中是最后是包含'\0'的。
    printf("%c\n", *"xyz");         //x
    printf("%c\n", "xyz"[2]);       //z

    return 0;
}
*/
