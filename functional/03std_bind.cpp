#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class BindClass
{
public:
    int Multi(int a, int b)
    {
        return a*b;
    }
};

//传引用
static void print(int& value)
{
    value ++;
    cout << "printf::value:" << value << endl;
}

//const 引用
static void print2(const int& value)
{
    cout << "printf::value:" << value << endl;
}

int main()
{
    BindClass bind_class;
    auto func = std::bind(&BindClass::Multi, &bind_class, std::placeholders::_1, std::placeholders::_2);
    cout << "func(2,4):" << func(2,4) << endl;


    int i_value = 20;
    auto func2 = std::bind(print, std::ref(i_value));   //引用时 
    func2();
    cout << "i_value:" << i_value << endl;

    //const 引用
    auto func3 = std::bind(print2, std::cref(i_value));   //引用时 
    func3();
    cout << "i_value:" << i_value << endl;

    return 0;
}