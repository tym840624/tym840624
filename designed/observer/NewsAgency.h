
#ifndef NEWAGENCY_H
#define NEWAGENCY_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>


//声明订阅类，只是做了声明，并没有包含这个类的头文件
class Observer;
class NewsAgency
{
public:
    void attach(Observer* ober);
    void detach(Observer* ober);
    virtual void notify(std::string msg) = 0;
    virtual ~NewsAgency(){}

protected:
    std::list<Observer*> m_list;
};

class YuLeNews : public NewsAgency
{
public:
    void notify(std::string msg) override;
};

class TiYuNews : public NewsAgency
{
public:
    void notify(std::string msg) override;
};

#endif