
#include<queue>
#include<thread>
#include<stdio.h>
#include<stdlib.h>
#include <mutex>
#include<thread>
#include<iostream>


using namespace std;

class TaskQueue
{
public:
    TaskQueue(const TaskQueue& other) = delete;
    TaskQueue& operator = (const TaskQueue& other) = delete;
    
    static TaskQueue* getInstance(){
        
        return &m_taskQ;
    }

    //判断任务队列是否为空
    bool isEmpty()
    {
        lock_guard<mutex> locker(m_mutex);
        return dataQ.empty();
    }
    //添加任务
    int addTask(int data)
    {
        lock_guard<mutex> locker(m_mutex);
        dataQ.push(data);
        return 0;
    }

    //删除任务
    bool delTask()
    {
        lock_guard<mutex> locker(m_mutex);
        if(!dataQ.empty()){     //这里不能用isEmpty()，否则重复加锁，变成的死锁
            dataQ.pop();
            return true;
        }else{
            return false;
        }
    }

    //取出任务
    int getTask()
    {
        lock_guard<mutex> locker(m_mutex);
        if(!dataQ.empty()){
           return dataQ.front();
        }else{
            return false;
        }
    }

private:
    TaskQueue() = default;
    queue<int> dataQ;
    static TaskQueue m_taskQ;
    mutex m_mutex;
};
TaskQueue TaskQueue::m_taskQ;

int main()
{
    //一个线程写任务，一个线程取任务
    thread t1([](){
        TaskQueue* ptr = TaskQueue::getInstance();
        for(int i= 0; i<100; i++){
            ptr->addTask(i+100);
            cout << "+++push task: " << i + 100 << ", threadID: " 
                << this_thread::get_id() << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    });

    thread t2([](){
        TaskQueue* ptr = TaskQueue::getInstance();
        this_thread::sleep_for(chrono::milliseconds(100));
        while(!ptr->isEmpty()){
            int data = ptr->getTask();
            cout << "---get task: " << data << ", threadID: " 
                << this_thread::get_id() << endl;
            ptr->delTask(); 
            this_thread::sleep_for(chrono::milliseconds(500));               
        }
    });

    t1.join();
    t2.join();
    return 0;

}


/*
https://subingwen.cn/cpp/mutex/#2-std-lock-guard

https://subingwen.cn/cpp/chrono/

https://subingwen.cn/cpp/thread/

https://subingwen.cn/cpp/this_thread/

*/