    
    #include "Observer.h"
    
Observer::Observer(string name, NewsAgency* news)
{
    m_name = name;
    m_news = news;
    m_news->attach(this);   //订阅
}
void Observer::unSubscribe()
{
    m_news->detach(this);
}

XiaoMing::XiaoMing(string name, NewsAgency* news):Observer(name, news){}    


void XiaoMing::update(string msg)
{
    cout << "@@@XiaoMing收到消息: " << msg << endl;
}

void XiaoHong::update(string msg)
{
    cout << "@@@XiaoHong收到消息: " << msg << endl;
}