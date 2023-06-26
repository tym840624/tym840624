#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define MAX 100

int number;

pthread_mutex_t mutex;


void* funcA(void *args)
{
    for(int i= 0; i<MAX; i++){
        pthread_mutex_lock(&mutex);
        int cur = number;
        cur++;
        usleep(100);
        number = cur;
        pthread_mutex_unlock(&mutex);
        printf("Thread A, id=%ld, number=%d\n", pthread_self(), number);
    }
    return NULL;
}

void* funcB(void *args)
{
    for(int i= 0; i<MAX; i++){
        pthread_mutex_lock(&mutex);
        int cur = number;
        cur++;
        usleep(100);
        number = cur;
        pthread_mutex_unlock(&mutex);
        printf("Thread B, id=%ld, number=%d\n", pthread_self(), number);
    }
    return NULL;
}

int main()
{
    pthread_t pid1;
    pthread_t pid2;
    
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&pid1, NULL, funcA, NULL);
    pthread_create(&pid2, NULL, funcB, NULL);

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}