
/*
在实现懒汉模式的单例的时候，相较于双重检查锁定模式有一种更简单的实现方法并且不会出现线程安全问题，那就是使用静态局部局部对象
在程序的第 9、10 行定义了一个静态局部队列对象，并且将这个对象作为了唯一的单例实例。使用这种方式之所以是线程安全的，是因为在 C++11 标准中有如下规定，并且这个操作是在编译时由编译器保证的：

如果指令逻辑进入一个未被初始化的声明变量，所有并发执行应当等待该变量完成初始化。

最后总结一下懒汉模式和饿汉模式的区别:
懒汉模式的缺点是在创建实例对象的时候有安全问题，但这样可以减少内存的浪费（如果用不到就不去申请内存了）。
饿汉模式则相反，在我们不需要这个实例对象的时候，它已经被创建出来，占用了一块内存。
对于现在的计算机而言，内存容量都是足够大的，这个缺陷可以被无视。

*/

class TaskQueue
{
public:
    TaskQueue(const TaskQueue* other) = delete;
    TaskQueue& operator = (const TaskQueue& other) = delete;

    static TaskQueue* getInstance()
    {
        static TaskQueue taskQ;
        return &taskQ;
    }

    void print()
    {
        cout << "hello, world!!!" << endl;
    }

private:
    TaskQueue() = default;
};

int main()
{
    TaskQueue* ptr = TaskQueue::getInstance();
    ptr->print();
    return 0;
}