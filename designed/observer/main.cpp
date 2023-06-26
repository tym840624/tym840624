#include "NewsAgency.h"

#include "Observer.h"

int main()
{
    YuLeNews* ylNew = new YuLeNews;
    TiYuNews* tyNew = new TiYuNews;

    XiaoHong* xhObserver = new XiaoHong("xiaohong", ylNew);
    XiaoMing* xmObserver = new XiaoMing("xiaoming", tyNew);

    ylNew->notify("小虎队复出...");

    cout << "------------------" << endl;

    tyNew->notify("乔丹复出...");

    delete ylNew;
    delete tyNew;
    delete xhObserver;
    delete xmObserver;
}

/*
YuLeNew 订阅人数: 1
@@@XiaoHong收到消息: 小虎队复出...
------------------
TiYuNew 订阅人数: 1
@@@XiaoMing收到消息: 乔丹复出...
*/