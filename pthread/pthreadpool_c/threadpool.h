#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <pthread.h>

typedef void(*TaskRun)(void*);

//任务结构体
typedef struct Task
{
    void(*function)(void* args);
    void* args;
}Task;


//线程池结构体
struct ThreadPool
{
    //任务队列
    Task* taskQ;
    int queueCapacity;      //容量
    int queueSize;      //当前任务个数
    int queueFront;     //队头--》取数据
    int queueRear;      //队尾--> 插入数据

    pthread_t managerID;        //管理者线程ID
    pthread_t* threadIDs;       //工作线程ID 

    int maxNum;     //最大线程数
    int minNum;     //最小线程数
    int busyNum;    //忙的线程数
    int liveNum;    //存活的线程数
    int exitNum;    //需要销毁的线程数

    pthread_mutex_t mutexPool;      //锁整个线程池
    pthread_mutex_t mutexBusy;      //锁busyNum变量
    pthread_cond_t  notFull;        //任务队列是不是满了
    pthread_cond_t  notEmpty;       //任务队列是不是空了

    int shutdown ;      //是不是要销毁线程池
};

typedef void(*TaskRun)(void* args);
typedef struct ThreadPool ThreadPool;

//创建线程池并初始化
ThreadPool* threadPoolCreate(int minNum, int maxNum, int queueSize);

//销毁线程池
void destroyThreadTool(ThreadPool* pool);

//添加任务
void threadPoolAdd(ThreadPool* pool,  TaskRun func, void* args);

//获取忙的工作线数
int getBusyNum(ThreadPool* pool);

//获取存活的线程数
int getLiveNum(ThreadPool* pool);

/////////
//工作的线程（消费者线程）任务函数
void* worker(void* args);

//管理者线程函数
void* manager(void* args);

//某个线程退出函数
void threadExit(ThreadPool* pool);

#endif