#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class ShipBody
{
public:
    virtual ~ShipBody(){}
    virtual string getShipBody() = 0;
};

class WoodBody : public ShipBody
{
public:
    string getShipBody() override {
        return string("用<木材>制作轮船船体...");
    }
};

class IronBody : public ShipBody
{
public:
    string getShipBody() override {
        return string("用<钢铁>制作轮船船体...");
    }
};

// 动力
class Engine
{
public:
    virtual string getEngine() = 0;
    virtual ~Engine() {}
};

class Human : public Engine
{
public:
    string getEngine() override
    {
        return string("使用<人力驱动>...");
    }
};

// 武器
class Weapon
{
public:
    virtual string getWeapon() = 0;
    virtual ~Weapon() {}
};

class Gun : public Weapon
{
public:
    string getWeapon() override
    {
        return string("配备的武器是<枪>");
    }
};

class Ship
{
public:
    Ship(Weapon* weapon, Engine* engine, ShipBody* body){
        m_weapon = weapon;
        m_engine = engine;
        m_body = body;
    }
    string getProperty()
    {
        string info = m_body->getShipBody() + m_weapon->getWeapon() + m_engine->getEngine();
        return info;
    }

    ~Ship(){
        delete m_weapon;
        delete m_engine;
        delete m_body;
    }

private:
    Weapon* m_weapon = nullptr;
    Engine* m_engine;
    ShipBody* m_body;
};

class AbstractFactory
{
public:
    AbstractFactory(){}
    virtual ~AbstractFactory() {}
    virtual Ship* createShip() = 0;
};

class BasicFactory : public AbstractFactory
{
public:
    Ship* createShip(){
        Ship* obj = new Ship( new Gun,  new Human, new WoodBody );
        cout << "<基础型>战船生产完毕, 可以下水啦..." << endl;
        return obj;
    }
};

int main()
{
    BasicFactory* baseFactory = new BasicFactory;
    Ship* ship = baseFactory->createShip();
    ship->getProperty();
    
    delete baseFactory;
    delete ship;
    return 0;
}
