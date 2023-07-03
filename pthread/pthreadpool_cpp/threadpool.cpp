  
#include "threadpool.h"

#include "task.h"
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;
#define NUMBER 2

ThreadPool::ThreadPool(int min, int max)
{
    //实例化任务队列
    m_taskQ = new TaskQueue;
    do
    {   
        m_minNum = min;
        m_maxNum = max;

        //给工作线程分配空间
        m_threadIDs = new pthread_t[m_maxNum];
        if(m_threadIDs == NULL)
        {
            cout << "new m_threadIDs faile..." << endl;
            break;
        }
        memset(m_threadIDs, 0, sizeof(pthread_t) * m_maxNum);


        if( pthread_mutex_init(&m_mutexPool, NULL) != 0 ||
            pthread_cond_init(&m_notEmpty, NULL) != 0)
        {
            cout << "init mutex or condition fail..." << endl;
            break;
        }

        //创建线程
        for(int i=0; i<m_minNum; i++)
        {
            pthread_create(&m_threadIDs[i], NULL, worker, this);
            cout << "创建子线程, ID:" << to_string((long long)m_threadIDs[i]) << endl;
        }

        //创建一个管理线程
        pthread_create(&m_managerID, NULL, manager, this);

    } while (0);
}
   
ThreadPool::~ThreadPool()
{
    m_shutdown = 1;

    //销毁管理线程
    pthread_join(m_managerID, NULL);

    //销毁工作线程
    for(int i=0; i<m_liveNum; i++)
    {
        pthread_cond_signal(&m_notEmpty);
    }

    //释放内存
    if(m_taskQ) delete m_taskQ;
    if(m_threadIDs) delete [] m_threadIDs;

    pthread_mutex_destroy(&m_mutexPool);
    pthread_cond_destroy(&m_notEmpty);
}

//添加任务
void ThreadPool::addTask(Task task)
{
    if(m_shutdown) return;

    //添加任务，此处不需要加锁，队列中有锁
    m_taskQ->addTask(task);

    //唤醒工作线程
    pthread_cond_signal(&m_notEmpty);
}

//获取忙的线程个数
int ThreadPool::getBusyNumber()
{
    int busyNumber = 0;
    pthread_mutex_lock(&m_mutexPool);
    busyNumber = m_busyNum;
    pthread_mutex_unlock(&m_mutexPool);
    return busyNumber;
}

//获取活着的线程
int ThreadPool::getLiveNumber()
{
    int liveNumber = 0;
    pthread_mutex_lock(&m_mutexPool);
    liveNumber = m_liveNum;
    pthread_mutex_unlock(&m_mutexPool);
    return liveNumber;
}

void* ThreadPool::worker(void* args)
{
   ThreadPool* pool = (ThreadPool*) args;

    //不断从任务列表中取出任务
    while(1)
    {
        pthread_mutex_lock(&pool->m_mutexPool);

        //如果队列为空，阻塞消费者线程
        while(pool->m_taskQ->taskNumber() == 0 && pool->m_shutdown != 1)
        {
            pthread_cond_wait(&pool->m_notEmpty, &pool->m_mutexPool);

            //判断是不是要销毁线程
            if( pool->m_exitNum > 0)
            {
                pool->m_exitNum --;
                if(pool->m_liveNum > pool->m_minNum)        //不能小于最小线程数
                {
                    pool->m_liveNum--;
                    pthread_mutex_unlock(&pool->m_mutexPool);
                    pool->threadExit();
                }
            }
        }

        //判断线程池是否关闭
        if( pool->m_shutdown)
        {
            pthread_mutex_unlock(&pool->m_mutexPool);
            pool->threadExit();
        }

        //从任务队列中取出一个任务
        Task task = pool->m_taskQ->takeTask();

        //工作线程+1
        pool->m_busyNum++;

        //线程池解锁
        pthread_mutex_unlock(&pool->m_mutexPool);

        //执行任务
        //cout << "thread id " << to_string((long long)pthread_self()) << " start working ..." << endl;
        printf("thread id %ld start working ...\n",pthread_self());

        task.function(task.args);
        delete task.args;
        task.args = nullptr;

        //cout << "thread id " << to_string((long long)pthread_self()) << " end work" << endl;
        printf("thread id %ld end work\n",pthread_self());
        pthread_mutex_lock(&pool->m_mutexPool);
        pool->m_busyNum--;
        pthread_mutex_unlock(&pool->m_mutexPool);
    }
    return nullptr;
}

void* ThreadPool::manager(void* args)
{
    ThreadPool* pool = (ThreadPool*) args;

    while(pool->m_shutdown != 1)
    {
        //每隔3s检测一次
        sleep(3);

        // 取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->m_mutexPool);
        int queueSize = pool->m_taskQ->taskNumber();
        int liveNum = pool->m_liveNum;
        int busyNum = pool->m_busyNum;
        pthread_mutex_unlock(&pool->m_mutexPool);

        // 添加线程
        // 任务的个数>存活的线程个数 && 存活的线程数<最大线程数
        if (queueSize > liveNum && liveNum < pool->m_maxNum)
        {
            pthread_mutex_lock(&pool->m_mutexPool);
            int counter = 0;
            for (int i = 0; i < pool->m_maxNum && counter < NUMBER
                && pool->m_liveNum < pool->m_maxNum; ++i)
            {
                if (pool->m_threadIDs[i] == 0)
                {
                    pthread_create(&pool->m_threadIDs[i], NULL, worker, pool);
                    counter++;
                    pool->m_liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->m_mutexPool);
        }

        // 销毁线程
        // 忙的线程*2 < 存活的线程数 && 存活的线程>最小线程数
        if (busyNum * 2 < liveNum && liveNum > pool->m_minNum)
        {
            pthread_mutex_lock(&pool->m_mutexPool);
            pool->m_exitNum = NUMBER;
            pthread_mutex_unlock(&pool->m_mutexPool);
            // 让工作的线程自杀
            for (int i = 0; i < NUMBER; ++i)
            {
                pthread_cond_signal(&pool->m_notEmpty);   
            }
        }
    }
    return NULL;
}

void ThreadPool::threadExit()
{
    pthread_t pid = pthread_self();
    for(int i=0; i<m_maxNum; i++)
    {
        if(m_threadIDs[i] == pid)
        {
            m_threadIDs[i] = 0;
            //cout << "threadExit() called " << to_string((long long)pthread_self()) << " exit ..." << endl;
            printf("threadExit() called %ld, exit ...\n", pthread_self());
            break;
        }
    }
    pthread_exit(NULL);
    return ;
}