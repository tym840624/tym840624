#ifndef _TASK_H_
#define _TASK_H_

#include <pthread.h>
#include <queue>

using callback = void(*)(void*);
typedef struct Task
{
    Task()
    {
        function = nullptr;
        args = nullptr;
    }

    Task(callback f, void* args)
    {
        function = f;
        this->args = args;
    }

    callback function;
    void* args;
} Task;

//任务队列
class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();

    //添加任务
    void addTask(Task& task);
    void addTask(callback f, void* args);

    //取出任务
    Task takeTask();

    //获取当前任务个数
    inline int taskNumber(){
        return m_queue.size();
    }

private:
    pthread_mutex_t m_mutex;        //互斥锁
    std::queue<Task> m_queue;       //任务队列
};
#endif