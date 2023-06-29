#include "json/json.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace Json;


void writeJson()
{
    // 将最外层的数组看做一个Value
    // 最外层的Value对象创建
    Value root;
    // Value有一个参数为int 行的构造函数
    root.append(12);	// 参数进行隐式类型转换
    root.append(12.34);
    root.append(true);
    root.append("tom");
    
    // 创建并初始化一个子数组
    Value subArray;
    subArray.append("jack");
    subArray.append("ace");
    subArray.append("robin");
    root.append(subArray);
    
    // 创建并初始化子对象
    Value subObj;
    subObj["sex"] = "woman";  // 添加键值对
    subObj["girlfriend"] = "lucy";
    root.append(subObj);
    
    // 序列化
#if 1
    // 有格式的字符串
    string str = root.toStyledString();
    cout << "1 str:" << str << endl;
#else
    FastWriter f;
    string str = f.write(root);
    cout << "0 str:" << str << endl;
#endif
    // 将序列化的字符串写磁盘文件
    ofstream ofs("test.json");
    ofs << str;
    ofs.close();
}

void readJson()
{
    // 1. 将磁盘文件中的json字符串读到磁盘文件
    ifstream ifs("test.json");
    // 2. 反序列化 -> value对象
    Value root;
    Reader r;
    r.parse(ifs, root);
    // 3. 从value对象中将数据依次读出
    if (root.isArray())
    {
        // 数组, 遍历数组
        for (int i = 0; i < root.size(); ++i)
        {
            // 依次取出各个元素, 类型是value类型
            Value item = root[i];
            // 判断item中存储的数据的类型
            if (item.isString())
            {
                cout << item.asString() << ", ";
            }
            else if (item.isInt())
            {
                cout << item.asInt() << ", ";
            }
            else if (item.isBool())
            {
                cout << item.asBool() << ", ";
            }
            else if (item.isDouble())
            {
                cout << item.asFloat() << ", ";
            }
            else if (item.isArray())
            {
                for (int j = 0; j < item.size(); ++j)
                {
                    cout << item[j].asString() << ", ";
                }
            }
            else if (item.isObject())
            {
                // 对象
                // 得到所有的key
                Value::Members keys = item.getMemberNames();
                for (int k = 0; k < keys.size(); ++k)
                {
                    cout << keys.at(k) << ":" << item[keys[k]] << ", ";
                }
            }
            
    	}
        cout << endl;
    }
}

int main()
{
    writeJson();
    readJson();
}

//g++ main.cpp -I ../include -L../lib -ljsoncpp -std=c++11 