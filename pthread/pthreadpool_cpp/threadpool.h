#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <pthread.h>
#include "task.h"

class TaskQueue;
class ThreadPool
{
public:
    ThreadPool(int min, int max);
    ~ThreadPool();

    //添加任务
    void addTask(Task task);

    //获取忙的线程个数
    int getBusyNumber();

    //获取活着的线程
    int getLiveNumber();

private:
    static void* worker(void* args);

    static void* manager(void* args);

    void threadExit();


private:
    TaskQueue* m_taskQ;           //任务队列
    pthread_t m_managerID;        //管理者线程ID
    pthread_t* m_threadIDs;       //工作线程ID 

    int m_maxNum;     //最大线程数
    int m_minNum;     //最小线程数
    int m_busyNum;    //忙的线程数
    int m_liveNum;    //存活的线程数
    int m_exitNum;    //需要销毁的线程数

    pthread_mutex_t m_mutexPool;      //锁整个线程池
    pthread_cond_t  m_notEmpty;       //任务队列是不是空了

    int m_shutdown ;      //是不是要销毁线程池
};

#endif