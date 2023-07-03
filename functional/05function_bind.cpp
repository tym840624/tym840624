#include <iostream>
#include <map>
using namespace std;

class DebugMain
{
public:
    using Factor = std::function<void(void)>;
    enum E_FUNC_TYPE
    {
        E_FUNC_1,
        E_FUNC_2,
    };

    DebugMain();

    //根据type来进行相应的函数调用
    void runFunc(int iType);

protected:
    void Func1();
    void Func2();

private:
    void initFuns();
    std::map<E_FUNC_TYPE, Factor> m_registerFuncs;
};

DebugMain::DebugMain()
{
    initFuns();
}

void DebugMain::initFuns()
{
    m_registerFuncs[E_FUNC_1] = std::bind(&DebugMain::Func1, this);
    m_registerFuncs[E_FUNC_2] = std::bind(&DebugMain::Func2, this);
}

void DebugMain::runFunc(int iType)
{
    std::map<E_FUNC_TYPE, Factor>::iterator iter;
    iter = m_registerFuncs.find((E_FUNC_TYPE)iType);
    if( iter != m_registerFuncs.end())
    {
        //cout << "start to call func" << endl;
        iter->second();
    }
}

void DebugMain::Func1()
{
    cout << "run Func1" << endl;
}
void DebugMain::Func2()
{
    cout << "run Func2" << endl;
}

int main()
{
    DebugMain debug_main;
    debug_main.runFunc(DebugMain::E_FUNC_2);
    return 0;
}
