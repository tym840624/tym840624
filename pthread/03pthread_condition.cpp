/*
场景描述：使用条件变量实现生产者和消费者模型，生产者有 5 个，往链表头部添加节点，消费者也有 5 个，删除链表头部的节点。
*/

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

pthread_cond_t cond;

pthread_mutex_t mutex;

struct Node
{
    int number;
    struct Node* next;
};

// 指向头结点的指针
struct Node * head = NULL;

void* product(void* args)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        //创建链表的新节点
        struct Node* pnew = (struct Node*)malloc(sizeof(struct Node));

        //节点初始化
        pnew->number = rand() % 1000;
        // 节点的连接, 添加到链表的头部, 新节点就新的头结点
        pnew->next = head;
        // head指针前移
        head = pnew;
        printf("+++producer, number = %d, tid = %ld\n", pnew->number, pthread_self());

        pthread_mutex_unlock(&mutex);

        //生产了任务，通知消费者
        pthread_cond_signal(&cond);

        //生成慢一点
        sleep(rand() % 3);
    }
}

void* consumer(void* args)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);

        while(head == NULL)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        // 取出链表的头结点, 将其删除
        struct Node* pnode = head;
        printf("--consumer: number: %d, tid = %ld\n", pnode->number, pthread_self());
        head = head->next;
        free(pnode);
        pthread_mutex_lock(&mutex);

        //消费慢一点
        sleep(rand() % 3);
    }
}


int main()
{
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_t ptid[5];
    pthread_t ctid[5];
    for(int i=0; i<5; i++)
    {
        pthread_create(&ptid[i], NULL, product, NULL);
    }

    for(int i=0; i<5; i++)
    {
        pthread_create(&ctid[i], NULL, consumer, NULL);
    }

    for(int i=0; i<5; i++)
    {
        pthread_join(ptid[5], NULL);
        pthread_join(ctid[5], NULL);
    }

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;
}