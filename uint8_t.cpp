#include <iostream>
#include <fstream>
#include <cstdint>
using namespace std;

int main() {
    // 写入文件    
    std::ofstream outfile("data.bin", std::ios::binary);
    if (!outfile) {
        std::cerr << "无法打开文件" << std::endl;
        return 1;
    }

    uint8_t data[] = {0xff, 0x00, 0x98, 0x01};
    cout << "size of data:" << sizeof(data) << endl;
    outfile.write(reinterpret_cast<char*>(data), sizeof(data));
    outfile.close();

    // 读取文件    
    std::ifstream infile("data.bin", std::ios::binary);
    if (!infile) {
        std::cerr << "无法打开文件" << std::endl;
        return 1;
    }

    uint8_t readData[4];
    infile.read(reinterpret_cast<char*>(readData), sizeof(readData));
    infile.close();

    // 打印输出    
    for (int i = 0; i < sizeof(readData); i++) {
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(readData[i]);
        if (i < sizeof(readData) - 1) {
            std::cout << ",";
        }
    }
    std::cout << std::endl;
    return 0;
}
