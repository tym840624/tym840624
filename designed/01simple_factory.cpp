/*
创建一个新的类，可以将这个类称之为工厂类。对于简单工厂模式来说，需要的工厂类只有一个。
在这个工厂类中添加一个公共的成员函数，通过这个函数来创建我们需要的对象，关于这个函数一般将其称之为工厂函数。
关于使用，首先创建一个工厂类对象，然后通过这个对象调用工厂函数，这样就可以生产出一个指定类型的实例对象了。

*/

class Sheep
{
public:
    void transform();
    void ability();

};

class Lion
{
  public:
    void transform();
    void ability();  
};

class Bat
{
public:
    void transform();
    void ability();
};

enum class Type{SHEEP, LION, BAT};

class SimpleFactory
{
public:
    SimpleFactory(){}
    ~SimpleFactory(){}

    void* createSimle(Type type){
        void* ptr = nullptr;
        switch (type){
            case Type::SHEEP:
                ptr = new Sheep;
                break;
            case Type::LION:
                ptr = new Lion;
                break;
            case Type::BAT:
                ptr = new Bat;
                break;
            default:
                break;
        }
        return ptr;
    }
};

int main()
{
    SimpleFactory* simple_factory_ptr = new SimpleFactory;
    Bat* obj = (Bat*)simple_factory_ptr->createSimle(Type::BAT);
    obj->ability();
    obj->transform();
    return 0;
}