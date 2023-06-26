
/*
懒汉模式是在类加载的时候不去创建这个唯一的实例，而是在需要使用的时候再进行实例化。

对于饿汉模式是没有线程安全问题的，因为在这种模式下访问单例对象的时候，这个对象已经被创建出来了。
要解决懒汉模式的线程安全问题，最常用的解决方案就是使用互斥锁。可以将创建单例对象的代码使用互斥锁锁住
*/
#include <thread.h>

#ifdef 0
class TaskQueue
{
public:
    TaskQueue(const TaskQueue* other) = delete;
    TaskQueue& operator = (const TaskQueue& other) = delete;

    //这个代码块被互斥锁锁住了，也就意味着不论有多少个线程，同时执行这个代码块的线程只能是一个（相当于是严重限行了，在重负载情况下，可能导致响应缓慢),多线程变成了线性执行了。
    //可以继续优化
    static TaskQueue* getInstance() 
    {
        m_mutex.lock();
        if(m_taskQ == nullptr){
            m_taskQ = new TaskQueue;
        }
        m_mutex.unlock();
        return m_taskQ;
    }

private:
    TaskQueue() = default;
    static TaskQueue* m_taskQ;
    static mutex m_mutex;
};

TaskQueue* TaskQueue::m_taskQ = nullptr;
mutex TaskQueue::m_mutex;

#endif

//双重检查锁定
#ifdef 1
class TaskQueue
{
public:
    TaskQueue(const TaskQueue* other) = delete;
    TaskQueue& operator = (const TaskQueue& other) = delete;

    //这个代码块被互斥锁锁住了，也就意味着不论有多少个线程，同时执行这个代码块的线程只能是一个（相当于是严重限行了，在重负载情况下，可能导致响应缓慢),多线程变成了线性执行了。
    //可以继续优化
    /*
    改进的思路就是在加锁、解锁的代码块外层有添加了一个 if判断（第 9 行），这样当任务队列的实例被创建出来之后，访问这个对象的线程就不会再执行加锁和解锁操作了
    两个嵌套的 if 来判断单例对象是否为空的操作就叫做双重检查锁定
    对于第一次创建单例对象的时候线程之间还是具有竞争关系，被互斥锁阻塞
    */
    static TaskQueue* getInstance() 
    {
        if(m_taskQ == nullptr)
        {
            m_mutex.lock();
            if(m_taskQ == nullptr){
                m_taskQ = new TaskQueue;
            }
            m_mutex.unlock();
        }
        return m_taskQ;
    }

private:
    TaskQueue() = default;
    static TaskQueue* m_taskQ;
    static mutex m_mutex;
};

TaskQueue* TaskQueue::m_taskQ = nullptr;
mutex TaskQueue::m_mutex;

#endif

//双重检查锁定的问题：原子操作问题,对应的机器指令可能会被重新排序
//https://subingwen.cn/design-patterns/singleton/
class TaskQueue
{
public:
    TaskQueue(const TaskQueue* other) = delete;
    TaskQueue& operator = (const TaskQueue& other) = delete;

    //这个代码块被互斥锁锁住了，也就意味着不论有多少个线程，同时执行这个代码块的线程只能是一个（相当于是严重限行了，在重负载情况下，可能导致响应缓慢),多线程变成了线性执行了。
    //可以继续优化
    /*
    改进的思路就是在加锁、解锁的代码块外层有添加了一个 if判断（第 9 行），这样当任务队列的实例被创建出来之后，访问这个对象的线程就不会再执行加锁和解锁操作了
    两个嵌套的 if 来判断单例对象是否为空的操作就叫做双重检查锁定
    对于第一次创建单例对象的时候线程之间还是具有竞争关系，被互斥锁阻塞

    在 C++11 中引入了原子变量 atomic，通过原子变量可以实现一种更安全的懒汉模式的单例
    */
    static TaskQueue* getInstance() 
    {
        TaskQueue* queue = m_taskQ.load();
        if(queue == nullptr)
        {
            //m_mutex.lock(); // 加锁: 方式1
            lock_guard<mutex> locker(m_mutex);      //// 加锁: 方式2
            queue = m_taskQ.load();
            if(queue == nullptr){
                queue = new TaskQueue;
                m_taskQ.store(queue);
            }

        }
        return queue;
    }

     void print()
    {
        cout << "hello, world!!!" << endl;
    }

private:
    TaskQueue() = default;
    static atomic<TaskQueue*> m_taskQ;
    static mutex m_mutex;
};

atomic<TaskQueue*> TaskQueue::m_taskQ;
mutex TaskQueue::m_mutex;

/*
上面代码中使用原子变量 atomic 的 store() 方法来存储单例对象，使用 load() 方法来加载单例对象。
在原子变量中这两个函数在处理指令的时候默认的原子顺序是 memory_order_seq_cst（顺序原子操作 - sequentially consistent），
使用顺序约束原子操作库，整个函数执行都将保证顺序执行，并且不会出现数据竞态（data races），
不足之处就是使用这种方法实现的懒汉模式的单例执行效率更低一些
*/

int main()
{
    TaskQueue* ptr = TaskQueue::getInstance();
    ptr->print();
    return 0;
}

//以上有线程安全问题