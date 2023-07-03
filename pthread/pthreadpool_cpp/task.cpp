#include "task.h"

TaskQueue::TaskQueue()
{
    pthread_mutex_init(&m_mutex, NULL);
}
TaskQueue::~TaskQueue()
{
    pthread_mutex_destroy(&m_mutex);
}

//添加任务
void TaskQueue::addTask(Task& task)
{
    pthread_mutex_lock(&m_mutex);
    m_queue.push(task);
    pthread_mutex_unlock(&m_mutex);
}

void TaskQueue::addTask(callback f, void* args)
{
    pthread_mutex_lock(&m_mutex);
    Task task;
    task.function = f;
    task.args = args;
    m_queue.push(task);
    pthread_mutex_unlock(&m_mutex);
}

//取出任务
Task TaskQueue::takeTask()
{
    Task t;
    pthread_mutex_lock(&m_mutex);
    if(m_queue.size() > 0)
    {
        t = m_queue.front();
        m_queue.pop();
    }
    pthread_mutex_unlock(&m_mutex);
    return t;
}