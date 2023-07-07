#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
    uint8_t v = 2;
    uint8_t v_1 = (v << 6) & 0xC0;
    cout << "v = " << v << ",v << 6 :" << v_1 << endl;

}