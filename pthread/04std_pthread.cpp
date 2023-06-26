#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

void func(int num, string& str)
{
    for (int i = 0; i < 10; ++i)
    {
        cout << "子线程: i = " << i << "num: " 
             << num << ", str: " << str << endl;
    }
}

void func2()
{
    for (int i = 0; i < 10; ++i)
    {
        cout << "子线程: i = " << i << endl;
    }
}

int main()
{
    cout << "main pthread id:" << this_thread::get_id() << endl;

    int num = thread::hardware_concurrency();
    cout << "CPU number: " << num << endl;
    
    thread t1(func, 520, "tym");
    thread t2(func2);

    cout << "t1 thread id:" << t1.get_id() << endl;
    cout << "t2 thread id:" << t2.get_id() << endl;

    t1.join();
    t2.join();

    return 0;
}