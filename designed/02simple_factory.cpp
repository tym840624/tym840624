/*
创建一个新的类，可以将这个类称之为工厂类。对于简单工厂模式来说，需要的工厂类只有一个。
在这个工厂类中添加一个公共的成员函数，通过这个函数来创建我们需要的对象，关于这个函数一般将其称之为工厂函数。
关于使用，首先创建一个工厂类对象，然后通过这个对象调用工厂函数，这样就可以生产出一个指定类型的实例对象了。

*/
class AbstractSmile
{
public:
    virtual void transform() {}
    virtual void ability() {}
    virtual ~AbstractSmile() {}
};

class SheepSmile : public AbstractSmile
{
public:
    void transform() override {

    }
    void ability() override {

    }

};

class LionSmile : public AbstractSmile
{
  public:
    void transform() override {

    }
    void ability() override {
        
    }
};

class BatSmile : public AbstractSmile
{
public:
    void transform() override {

    }
    void ability() override {
        
    }
};

enum class Type{SHEEP, LION, BAT};

class SimpleFactory
{
public:
    SimpleFactory(){}
    ~SimpleFactory(){}

    AbstractSmile* createSimle(Type type){
        AbstractSmile* ptr = nullptr;
        switch (type){
            case Type::SHEEP:
                ptr = new SheepSmile;
                break;
            case Type::LION:
                ptr = new LionSmile;
                break;
            case Type::BAT:
                ptr = new BatSmile;
                break;
            default:
                break;
        }
        return ptr;
    }
};

int main()
{
    SimpleFactory* factory_ptr = new SimpleFactory;
    AbstractSmile* obj = factory_ptr->createSimle(Type::BAT);
    obj->ability();
    obj->transform();
    return 0;
}
/*
由于人造恶魔果实类有继承关系， 并且实现了多态，所以父类的析构函数也应该是虚函数，这样才能够通过父类指针或引用析构子类的对象。
工厂函数 createSmile(Type type) 的返回值修改成了 AbstractSmile* 类型，这是人造恶魔果实类的基类，通过这个指针保存的是子类对象的地址，
这样就实现了多态，所以在 main() 函数中，通过 obj 对象调用的实际是子类 BatSmile 中的函数

https://subingwen.cn/design-patterns/simple-factory/
*/