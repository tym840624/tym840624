#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

using namespace std;

struct Data {
    uint8_t* ptr;
    int length;
    time_t timestamp;
};

vector<Data> data_vec;

void print_and_delete_expired_data() {
    time_t current_time = time(NULL);
    int count = 0;

    for (vector<Data>::iterator it = data_vec.begin(); it != data_vec.end() && count < 5; ) {
        if (current_time - it->timestamp > 5) {
            cout << "Expired data: ";
            for (int i = 0; i < it->length; i++) {
                cout << (int)it->ptr[i] << " ";
            }
            cout << endl;
            delete[] it->ptr;
            it = data_vec.erase(it);
            count++;
        } else {
            it++;
        }
    }
}

void generate_data() {
    int length = rand() % 100 + 1;
    uint8_t* ptr = new uint8_t[length];
    for (int i = 0; i < length; i++) {
        ptr[i] = rand() % 256;
    }
    Data data = {ptr, length, time(NULL)};
    data_vec.push_back(data);
    cout << "data_vec size:" << data_vec.size() << endl;


    //超过数组大小超过20时，每次取出5个，判断超过5秒的数据先打印输出，再释放内存，并删除结点
    if (data_vec.size() > 100) {

        time_t current_time = time(NULL);
        int count = 0;

        for (vector<Data>::iterator it = data_vec.begin(); it != data_vec.end() && count < 25; ) {
            if (current_time - it->timestamp > 1) {
                cout << "time:" << it->timestamp << ",Expired data: ";
                for (int i = 0; i < 10; i++) {
                    cout << (int)it->ptr[i] << " ";
                }
                cout << endl;
                delete[] it->ptr;
                it = data_vec.erase(it);
                count++;
            } else {
                it++;
            }
        }
        cout << "data_vec size:" << data_vec.size() << endl;
        //data_vec.erase(data_vec.begin(), data_vec.begin() + 5);
    }
}

int main() {
    srand(time(NULL));
    while (true) {
        chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();
        generate_data();
        //print_and_delete_expired_data();
        
        
        usleep(40000);
        chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        cout << "spend:" << chrono::duration_cast<chrono::milliseconds>(time_end - time_start).count() << " ms, ";
    }
    return 0;
}
