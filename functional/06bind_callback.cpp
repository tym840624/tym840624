#include <iostream>
#include <map>
using namespace std;

class DebugCallback
{
public:
    DebugCallback(std::function<void()> callback) : m_callback(callback)
    {}
    
    void start()
    {
        m_callback();
    }

    static void Callback01()
    {
        cout << "static callback01()" << endl;
    }
    
    static void Callback02()
    {
        cout << "static callback02()" << endl;
    }   
private:
    std::function<void()> m_callback;
};


int main()
{
    DebugCallback debug_call1(std::bind(&DebugCallback::Callback01));
    debug_call1.start();

    DebugCallback debug_call2(std::bind(&DebugCallback::Callback02));
    debug_call2.start();
    return 0;
}

