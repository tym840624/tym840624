#include<iostream>
#include<functional>

using namespace std;

void call_when_even(int x, const std::function<void(int)> fun)
{
    if(x%2==0){
        fun(x);
    }
}

void output_1(int x)
{
    cout << "output_1:" << x << endl;
}

void output_add_2(int x)
{
    cout << "output_add_2:" << x+2 << endl;
}

int main()
{
    std::function<void(int)> fun1 = std::bind(output_1, std::placeholders::_1);
    for(int i=0; i<10; i++)
    {
        call_when_even(i, fun1);
    }

    auto fun2 = std::bind(output_add_2, std::placeholders::_1);
    for(int i=0; i<10; i++)
    {
        call_when_even(i, fun2);
    }
    
    return 0;
}