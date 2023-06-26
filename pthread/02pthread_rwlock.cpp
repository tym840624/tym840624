/*
8 个线程操作同一个全局变量，3 个线程不定时写同一全局资源，5 个线程不定时读同一全局资源
*/


#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int number;

pthread_rwlock_t rwlock;

void* write_number(void* args)
{
    while(1)
    {
        pthread_rwlock_wrlock(&rwlock);
        int cur = number;
        cur ++;
        number = cur;
        printf("++写操作完毕, number : %d, tid = %ld\n", number, pthread_self());
        pthread_rwlock_unlock(&rwlock);
        // 添加sleep目的是要看到多个线程交替工作
        usleep(rand() % 100);
    }
}

void* read_number(void* args)
{
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("--读操作完毕, number : %d, tid = %ld\n", number, pthread_self());
        pthread_rwlock_unlock(&rwlock);
        // 添加sleep目的是要看到多个线程交替工作
        usleep(rand() % 100);     
    }
}

int main()
{
    pthread_t wtid[3];
    pthread_t rtid[5];

    pthread_rwlock_init(&rwlock, NULL);

    for(int i=0; i<3; i++)
    {
        pthread_create(&wtid[i], NULL, write_number, NULL);
    }

    for(int i=0; i<5; i++)
    {
        pthread_create(&rtid[i], NULL, read_number, NULL);
    }

     // 释放资源
    for(int i=0; i<3; ++i)
    {
        pthread_join(wtid[i], NULL);
    }

    for(int i=0; i<5; ++i)
    {
        pthread_join(rtid[i], NULL);
    }


    pthread_rwlock_destroy(&rwlock);

    return 0;
}