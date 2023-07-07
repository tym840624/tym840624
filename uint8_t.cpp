#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    uint8_t* ptr = (uint8_t *)"hello";
    int length = strlen((char *)ptr);
    for (int i = 0; i < length; i++) {
        cout << (int)ptr[i] << " ";
    }
    cout << "--------------" << endl;
    //16进制输出
    for (int i = 0; i < length; i++) {
        cout << std::uppercase << std::setfill('0') << std::setw(2)
            << std::hex << ((*(ptr + i)) & 0xff) << " ";
    }
    cout << endl;
}