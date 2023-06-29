#include <iostream>

using namespace std;

#define LOG_ENABLED 0
int main()
{
    #if LOG_ENABLED == 1
    cout << "输出日志" << endl;
    #endif

    #ifdef LOG_ENABLED 
    cout << "只要定义就输出" << endl;
    #endif

    cout << "正常输出" << endl;
    return 0;
}