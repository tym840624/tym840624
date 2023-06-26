/*
创建一个新的类，可以将这个类称之为工厂类。对于简单工厂模式来说，需要的工厂类只有一个。
在这个工厂类中添加一个公共的成员函数，通过这个函数来创建我们需要的对象，关于这个函数一般将其称之为工厂函数。
关于使用，首先创建一个工厂类对象，然后通过这个对象调用工厂函数，这样就可以生产出一个指定类型的实例对象了。

*/

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <mutex>
#include <thread>
#include <iostream>

using namespace std;


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
        std::cout << "SheepSmile::transform()" << std::endl;
    }
    void ability() override {
        std::cout << "SheepSmile::ability()" << std::endl;
    }

};

class LionSmile : public AbstractSmile
{
  public:
    void transform() override {
        std::cout << "LionSmile::transform()" << std::endl;
    }
    void ability() override {
        std::cout << "LionSmile::ability()" << std::endl   ;  
    }
};

class BatSmile : public AbstractSmile
{
public:
    void transform() override {
        std::cout << "BatSmile::transform()" << std::endl;
    }
    void ability() override {
        std::cout << "BatSmile::ability()" << std::endl     ;    
    }
};

enum class Type{SHEEP, LION, BAT};
class AbstractFactory
{
public:
    AbstractFactory(){}
    virtual ~AbstractFactory(){}
    virtual AbstractSmile* createSmile() = 0;
};

class SheepFactory : public AbstractFactory
{
public:
    SheepFactory(){}
    AbstractSmile* createSmile(){
        AbstractSmile* ptr = new SheepSmile;
        return ptr;
    }
};

class LionFactory : public AbstractFactory
{
public:
    AbstractSmile* createSmile(){
        AbstractSmile* ptr = new LionSmile;
        return ptr;
    }
};

class BatFactory : public AbstractFactory
{
public:
    AbstractSmile* createSmile(){
        AbstractSmile* ptr = new BatSmile;
        return ptr;
    }
};

int main()
{
    SheepFactory* factory_ptr = new SheepFactory;
    AbstractSmile* obj = factory_ptr->createSmile();
    obj->ability();
    obj->transform();
    return 0;
}
