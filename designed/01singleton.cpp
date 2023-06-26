/*
https://subingwen.cn/design-patterns/singleton/

*/
/*
class Singleton
{
public:
    // = delete 代表函数禁用, 也可以将其访问权限设置为私有
    Singleton(const Singleton& other) = delete;
    Singleton& operator = (const Singleton& other) = delete;
    static Singleton* getInstance();

private:
    Singleton() = defalut;
    static Singleton* m_obj;
};
*/