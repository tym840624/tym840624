#include "NewsAgency.h"
#include "Observer.h"	// 在源文件中包含该头文件

void NewsAgency::attach(Observer* ober)
{
    m_list.push_back(ober);
}
void NewsAgency::detach(Observer* ober)
{
    m_list.remove(ober);
}


void YuLeNews::notify(std::string msg)
{
    std::cout << "YuLeNew 订阅人数: " << m_list.size() << std::endl;
    for(const auto& item : m_list){
        item->update(msg);
    }
}


void TiYuNews::notify(std::string msg)
{
    std::cout << "TiYuNew 订阅人数: " << m_list.size() << std::endl;
    for(const auto& item : m_list){
        item->update(msg);
    }
}