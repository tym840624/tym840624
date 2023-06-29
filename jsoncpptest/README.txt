
下载jsoncpp源码：

git clone https://github.com/open-source-parsers/jsoncpp.git

2.进入jsoncpp目录
cd jsoncpp

3.创建一个构建目录，并进入该目录
mkdir build
cd build

4.使用cmake命令生成Makefile：
cmake ..

5.使用make命令编译源代码并生成库文件：
make

6.将得到头文件，以及编译生成的库文件libjsoncpp.a拷贝到相应目录

写测试代码包含：
#include <json/json.h>

7.编译时提定目录
g++ main.cpp -I ../include -L../lib -ljsoncpp -std=c++11 
