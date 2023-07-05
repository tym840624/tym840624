#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;
/*
//多线程计算1+2+...+100
std::mutex g_mutex;
void Add(int& num, int& sum)
{
    while(true)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        if(num < 100){
            num++;
            sum += num;
            cout << "thread id:" << std::this_thread::get_id() << ", num: "<< num << ", sum: " << sum << endl;
        }
        else
        {
            break;
        }
    }
}
int main()
{
    int sum = 0;
    int num = 0;
    std::vector<std::thread> ver ;  //保存线程
    for(int i=0; i<20; i++)
    {
        std::thread t = std::thread(Add, std::ref(num), std::ref(sum));
        ver.emplace_back(std::move(t));
    }

    std::for_each(ver.begin(), ver.end(), std::mem_fn(&std::thread::join));
    cout << "sum:" << sum << endl;
    return 0;
}

*/


std::mutex mtx;           // mutex for critical section
std::once_flag flag;

void print_block (int n, char c) {
    //unique_lock有多组构造函数, 这里std::defer_lock不设置锁状态
    std::unique_lock<std::mutex> my_lock (mtx, std::defer_lock);
    //尝试加锁, 如果加锁成功则执行
    //(适合定时执行一个job的场景, 一个线程执行就可以, 可以用更新时间戳辅助)
    if(my_lock.try_lock()){
        cout << "print_block() thread id:" << std::this_thread::get_id() << endl;
        for (int i=0; i<n; ++i)
            std::cout << c;
        std::cout << '\n';
    }
}

void run_one(int &n){
    cout << "run_one() thread id:" << std::this_thread::get_id() << endl;
    std::call_once(flag, [&n]{n=n+1; cout << "run_one() n:" << n << endl;}); //只执行一次, 适合延迟加载; 多线程static变量情况
}

int main () {
    std::vector<std::thread> ver;
    int num = 0;
    for (auto i = 0; i < 10; ++i){
        ver.emplace_back(print_block,50,'*');
        ver.emplace_back(run_one, std::ref(num));
    }

    for (auto &t : ver){
        t.join();
    }
    std::cout << num << std::endl;
    return 0;
}