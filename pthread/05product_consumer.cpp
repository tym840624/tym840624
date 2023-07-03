/*
C++11 提供了两种条件变量：
condition_variable：需要配合 std::unique_lock<std::mutex> 进行 wait 操作，也就是阻塞线程的操作。
condition_variable_any：可以和任意带有 lock()、unlock() 语义的 mutex 搭配使用，也就是说有四种：
std::mutex：独占的非递归互斥锁
std::timed_mutex：带超时的独占非递归互斥锁
std::recursive_mutex：不带超时功能的递归互斥锁
std::recursive_timed_mutex：带超时的递归互斥锁
条件变量通常用于生产者和消费者模型，大致使用过程如下：

拥有条件变量的线程获取互斥量
循环检查某个条件，如果条件不满足阻塞当前线程，否则线程继续向下执行
产品的数量达到上限，生产者阻塞，否则生产者一直生产。。。
产品的数量为零，消费者阻塞，否则消费者一直消费。。。
条件满足之后，可以调用 notify_one() 或者 notify_all() 唤醒一个或者所有被阻塞的线程
由消费者唤醒被阻塞的生产者，生产者解除阻塞继续生产。。。
由生产者唤醒被阻塞的消费者，消费者解除阻塞继续消费。。。


https://subingwen.cn/cpp/condition/
*/

#include <condition_variable>
#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <functional>
#include <sstream>

using namespace std;

class SyncQueue
{
public:
    SyncQueue(int maxSize) : m_maxSize(maxSize) {}
    
    void put(int data)
    {
        unique_lock<mutex> locker(m_mutex);
        while(m_queue.size() == m_maxSize)
        {
            //阻塞队列
            cout << "队列己满，请耐心等待。。。" << endl;
            m_notFull.wait(locker);     //阻塞生产线程
        }
        //将任务放入队列
        m_queue.push_back(data);
        std::thread::id pid = std::this_thread::get_id();
        cout << threadIdToString(pid) << ", 任务ID: " << data << " 己生产，请消费。" << endl;

        //通知消费者消费
        m_notEmpty.notify_one();            //通知可继续消费
    }

    void take()
    {
        unique_lock<mutex> locker(m_mutex);
        while(m_queue.empty())
        {
            cout << "队列为空，请耐心等待。。。" << endl;
            m_notEmpty.wait(locker);        //阻塞消费线程
        }

        int value = m_queue.front();  
        std::thread::id pid = std::this_thread::get_id();
        cout << threadIdToString(pid) << ", 任务ID: " << value << " 己被消费，请继续生产。" << endl;
        m_queue.pop_front();
        m_notFull.notify_one();             //通知可继续生产
    }

    std::string threadIdToString(std::thread::id& pid)
    {
        std::ostringstream oss;
        oss << pid;
        return oss.str();
    }
private:
    list<int> m_queue;
    int m_maxSize;
    mutex m_mutex;
    condition_variable m_notEmpty;      //队列不为空
    condition_variable m_notFull;       //队列满

};

void producer(SyncQueue& syncQueue, int start, int end)
{
    //cout << "start product task id[" << start << "," << end << "]" << endl;
    for (int i = start; i <= end; i++)
    {
        syncQueue.put(i);
    }
}

void consumer(SyncQueue& syncQueue, int itemCount)
{
    for (int i = 0; i < itemCount; i++)
    {
        syncQueue.take();
    }
}

    std::string threadIdToString(std::thread::id& pid)
    {
        std::ostringstream oss;
        oss << pid;
        return oss.str();
    }

int main()
{
    const int taskCount = 50;
    const int producerCount = 3;
    const int consumerCount = 3;
    const int itemCountPerConsumer = taskCount / consumerCount;

    SyncQueue syncQueue(10);

    std::thread producerThreads[producerCount];
    std::thread consumerThreads[consumerCount];

    // 创建生产者线程
    for (int i = 0; i < producerCount; i++)
    {
        int start = i * (taskCount / producerCount) + 1;
        int end = (i + 1) * (taskCount / producerCount);
        producerThreads[i] = std::thread(producer, std::ref(syncQueue), start, end);
    }

    // 创建消费者线程
    for (int i = 0; i < consumerCount; i++)
    {
        consumerThreads[i] = std::thread(consumer, std::ref(syncQueue), itemCountPerConsumer);
    }

    for (int i = 0; i < producerCount; i++)
    {
        producerThreads[i].join();      // 等待生产者线程完成
        consumerThreads[i].join();      // 等待消费者线程完成
    }

    std::thread::id threadId = std::this_thread::get_id();
    cout << "main thread Id:" << threadIdToString(threadId) << endl;

    cout << "主线程的线程ID: " << this_thread::get_id() << endl;
    
    return 0;
}

/*
int main()
{
    SyncQueue sync_queue(50);

    //3个生成线程，3个消费线程
    auto product = std::bind(&SyncQueue::put, &sync_queue, std::placeholders::_1);
    auto consumer = std::bind(&SyncQueue::take, &sync_queue);

    thread ptid[3];
    thread ctid[3];

    for(int i=0; i<3; i++)
    {
        ptid[i] = thread(product, i+100);
        ctid[i] = thread(consumer);
    }

    for(int i=0; i<3; i++)
    {
        ptid[i].join();
        ctid[i].join();
    }
    return 0;
}
*/