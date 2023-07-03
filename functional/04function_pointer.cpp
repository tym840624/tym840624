/*
函数指针变量用于存储函数指针，以便后续的调用。有时候可以利用它实现多个消息对象的处理，并且一定程度满足开闭原则。
首先实现抽象基类JAbstractBaseTest，接着再实现继承JAbstractBaseTest的两个子类JObjA和JObjB
*/
#include <iostream>
#include <map>

using namespace std;

class AbstractBase
{
public:
    AbstractBase(){}
    virtual ~AbstractBase(){}

    virtual void run() = 0;
};

class JobA : public AbstractBase
{
public:
    JobA()
    {
        cout << "JobA constructor" << endl;
    }
    ~JobA()
    {
        cout << "JobA desconstructor" << endl;
    }
    void run()
    {
        cout << "JobA::run()" << endl;
    }
    static AbstractBase* createInstance()
    {
        return new JobA();
    }
};

class JobB : public AbstractBase
{
public:
    JobB()
    {
        cout << "JobA constructor" << endl;
    }
    ~JobB()
    {
        cout << "JobB desconstructor" << endl;
    }
    
    void run()
    {
        cout << "JobB::run()" << endl;
    }
    static AbstractBase* createInstance()
    {
        return new JobB();
    }
};

//测试框架
class DebugMain
{
public:
    DebugMain()
    {
        initJob();
    }
    typedef AbstractBase* (*createObj)();

    enum E_JOB_TYPE
    {
        E_JOB_A,
        E_JOB_B,
    };

    void initJob()
    {
        m_mapRegisterClass[E_JOB_A] = JobA::createInstance;
        m_mapRegisterClass[E_JOB_B] = JobB::createInstance;
    }

    //通过ID从映射表中获取任务对象
    AbstractBase* getObj(E_JOB_TYPE eObjType)
    {
        std::map<E_JOB_TYPE, createObj>::iterator iter;
        iter = m_mapRegisterClass.find(eObjType);
        if( iter != m_mapRegisterClass.end())
        {
            return iter->second();
        }
        return nullptr;
    }

private:
    std::map<E_JOB_TYPE, createObj> m_mapRegisterClass;
};

int main()
{
    DebugMain debug_main;
    AbstractBase* abstractBase = debug_main.getObj(DebugMain::E_JOB_A);
    abstractBase->run();
    return 0;
}