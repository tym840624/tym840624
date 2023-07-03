#include "threadpool.h"
#include "task.h"
#include <iostream>
using namespace std;

#include <unistd.h>

void taskFunc(void* args)
{
    int num = *(int*)args;
    cout << "thread " <<  to_string((long long)pthread_self()) << " is working..., number:" << num << endl;
    sleep(1);
    return ;
}

int main()
{
    ThreadPool threadPool(3, 10);

    for(int i=0; i<50; i++)
    {
        Task task;
        task.function = taskFunc;

        int* num = (int*)malloc(sizeof(int));
        *num = i + 100;
        task.args = num;
        threadPool.addTask(task);
    }

    sleep(30);
    return 0;
}