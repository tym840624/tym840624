
#include "threadpool.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMBER 2
//创建线程池并初始化
ThreadPool* threadPoolCreate(int minNum, int maxNum, int queueSize)
{
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));

    do{
        if(pool == NULL)
        { 
            printf("malloc pthread pool faile\n"); 
            break;
        }

        pool->threadIDs = (pthread_t *)malloc(sizeof(pthread_t)* maxNum);
        memset(pool->threadIDs, 0, sizeof(pthread_t) * maxNum);

        pool->minNum = minNum;
        pool->maxNum = maxNum;
        pool->busyNum = 0;
        pool->liveNum = minNum;
        pool->exitNum = 0;

        if( pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
            pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
            pthread_cond_init(&pool->notFull, NULL) != 0 ||
            pthread_cond_init(&pool->notEmpty, NULL) != 0)
        {
            printf("init mutex or cond faile\n"); 
            break;
        }

        //任务列表
        pool->taskQ = (Task*)malloc(sizeof(Task) * queueSize);
        pool->queueCapacity = queueSize;
        pool->queueFront = 0;
        pool->queueRear = 0;
        pool->queueSize = 0;

        pool->shutdown = 0;

        //创建管理线程
        pthread_create(&pool->managerID, NULL, manager, pool);      //把pool线程池指针传入线程函数中

        //创建工作线程
        for(int i=0; i<minNum; i++)
        {
            pthread_create(&pool->threadIDs[i], NULL, worker, pool);    //把pool线程池指针传入线程函数中
        }
        return pool;
    }while(1);
    
    if(pool && pool->threadIDs)
    {
        free(pool->threadIDs);
    }

    if(pool && pool->taskQ)
    {
        free(pool->taskQ);
    }
    if(pool) { free(pool); }

    return NULL;
}

//销毁线程池
void destroyThreadTool(ThreadPool* pool)
{
    if(pool == NULL)
    {
        return;
    }
    
    //关闭线程池
    pool->shutdown = 1;

    //阻塞回收管理线程
    pthread_join(pool->managerID, NULL);

    //唤醒阻塞的消费线程
    for(int i=0; i<pool->liveNum; i++)
    {
        pthread_cond_signal(&pool->notEmpty);
    }

    //释放堆内存
    if(pool && pool->threadIDs)
    {
        free(pool->threadIDs);
    }

    if(pool && pool->taskQ)
    {
        free(pool->taskQ);
    }

    //销毁互斥锁与条件量
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    free(pool);
    return;
}

//添加任务
void threadPoolAdd(ThreadPool* pool,  TaskRun func, void* args)
{
    //锁整个线程池
    pthread_mutex_lock(&pool->mutexPool);

    //队列满且不关闭线程池的情况下，阻塞生产者线程
    while(pool->queueSize == pool->queueCapacity && pool->shutdown != 1)
    {
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }

    if(pool->shutdown)
    {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }

    //添加任务
    pool->taskQ[pool->queueRear].function = func;
    pool->taskQ[pool->queueRear].args = args;
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
    pool->queueSize++;
    
    int num = *(int*)args;
    printf("add success. thread %ld, num = %d\n", pthread_self(), num);

    //通知消费者线程
    pthread_cond_signal(&pool->notEmpty);
    
    pthread_mutex_unlock(&pool->mutexPool);
    return ;
}

//获取忙的工作线数
int getBusyNum(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNum = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}

//获取存活的线程数
int getLiveNum(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int liveNum = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return liveNum;
}

/////////
//工作的线程（消费者线程）任务函数
void* worker(void* args)
{
    ThreadPool* pool = (ThreadPool*) args;

    //不断从任务列表中取出任务
    while(1)
    {
        pthread_mutex_lock(&pool->mutexPool);

        //如果队列为空，阻塞消费者线程
        while(pool->queueSize == 0 && pool->shutdown != 1)
        {
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

            //判断是不是要销毁线程
            if( pool->exitNum > 0)
            {
                pool->exitNum --;
                if(pool->liveNum > pool->minNum)        //不能小于最小线程数
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    threadExit(pool);
                }
            }
        }

        //判断线程池是否关闭
        if( pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }

        //从任务队列中取出一个任务
        Task task;
        task.function = pool->taskQ[pool->queueFront].function;
        task.args = pool->taskQ[pool->queueFront].args;

        //移动头结点
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
        pool->queueSize --;

        //解锁, 解除生产者线程的阻塞状态
        pthread_cond_signal(&pool->notFull);
        pthread_mutex_unlock(&pool->mutexPool);
        
        printf("thread %ld start working ...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);

        task.function(task.args);
        free(task.args);
        task.args = NULL;

        printf("thread %ld end working ...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

//管理者线程函数
void* manager(void* args)
{
    ThreadPool* pool = (ThreadPool*) args;

    while(pool->shutdown != 1)
    {
        //每隔3s检测一次
        sleep(3);

        // 取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum;
        pthread_mutex_unlock(&pool->mutexPool);

        // 取出忙的线程的数量
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);


        // 添加线程
        // 任务的个数>存活的线程个数 && 存活的线程数<最大线程数
        if (queueSize > liveNum && liveNum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            for (int i = 0; i < pool->maxNum && counter < NUMBER
                && pool->liveNum < pool->maxNum; ++i)
            {
                if (pool->threadIDs[i] == 0)
                {
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }

        // 销毁线程
        // 忙的线程*2 < 存活的线程数 && 存活的线程>最小线程数
        if (busyNum * 2 < liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            // 让工作的线程自杀
            for (int i = 0; i < NUMBER; ++i)
            {
                pthread_cond_signal(&pool->notEmpty);       //这个是一个技巧
            }
        }
    }
    return NULL;
}

//某个线程退出函数
void threadExit(ThreadPool* pool)
{
    pthread_t pid = pthread_self();
    for(int i=0; i<pool->maxNum; i++)
    {
        if(pool->threadIDs[i] == pid)
        {
            pool->threadIDs[i] = 0;
            printf("threadExit() called, %ld exit ...\n", pid);
            break;
        }
    }
    pthread_exit(NULL);
    return ;
}

void* taskFunc(void* args)
{
    int num = *(int*)args;
    printf("thread %ld is working..., number=%d\n", pthread_self(), num);
    sleep(1);
}

int main()
{
    //创建线程池
    ThreadPool* pool = threadPoolCreate(3, 10, 20);

    //添加任务
    for(int i=0; i<100; i++)
    {
        int* num = (int*)malloc(sizeof(int));
        *num = i + 100;
        threadPoolAdd(pool, taskFunc, num);
    }

    sleep(30);
    destroyThreadTool(pool);
    return 0;
}
