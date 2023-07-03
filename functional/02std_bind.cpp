#include<iostream>
#include<functional>

//https://cloud.tencent.com/developer/article/1474565

using namespace std;
typedef std::function<void(int)> PrintFunction;
void print(const char* text, PrintFunction callback, int x)
{
    printf("%s\n", text);
    if(callback){
        callback(x);
    }
}

//类成员函数
class Test
{
public:
    void PrintInter(int iRet)
    {
        cout << "Test::" << iRet << endl;
    }
};

//函数签名不一样的函数
void print2(int iRet1, int iRet2)
{
    cout << "print2:" << iRet1 << "," << iRet2 << endl;
}

int main()
{
    Test obj;
    auto fun1 = std::bind(&Test::PrintInter, &obj, std::placeholders::_1);
    print("func1", fun1, 10);

    auto fun2 = std::bind(&print2, std::placeholders::_1, 100);
    print("func2", fun2, 2);

}