
/*
饿汉模式就是在类加载的时候立刻进行实例化
定义这个单例类的时候，就把这个静态的单例对象创建出来了。当使用者通过 getInstance() 获取这个单例对象的时候，它已经被准备好了

饿汉模式在多线程中不会有线程安全问题
*/

class TaskQueue
{
public:
    TaskQueue(const TaskQueue& other) = delete;
    TaskQueue& operator = (const TaskQueue& other) = delete;
    static TaskQueue* getInstance()
    {
        return m_taskQ;
    }
private:
    TaskQueue() = default;
    static TaskQueue* m_taskQ;
};

// 静态成员初始化放到类外部处理
TaskQueue* TaskQueue::m_taskQ = new TaskQueue;

int main()
{
    TaskQueue* obj = TaskQueue::getInstance();
}

