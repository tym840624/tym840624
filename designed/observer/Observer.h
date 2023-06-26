#ifndef  OBERSERVER_H
#define OBERSERVER_H

#include <string>
#include <iostream>
#include "NewsAgency.h"
using namespace std;

class Observer
{
public:
    Observer(string name, NewsAgency* news);
    void unSubscribe();
    virtual ~Observer(){}
    virtual void update(string name) = 0;
private:
    string m_name;
    NewsAgency* m_news;
};

class XiaoMing : public Observer
{
public:
    //using Observer::Observer;
    XiaoMing(string name, NewsAgency* news);
    void update(string msg) override;
};

class XiaoHong : public Observer
{
public:
    using Observer::Observer;
    void update(string msg) override;
};

#endif