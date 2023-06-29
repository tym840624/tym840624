#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unistd.h>


using namespace std;
class DataPool {
public:
    DataPool(uint32_t size) {
        _data_buf = (uint8_t *)malloc(size);
        _capacity = size;
        reset();
    }

    virtual ~DataPool() {
        free(_data_buf);
    }

    int32_t add(const uint8_t * src, uint32_t size) {
        if (nullptr == src) {
            return -1;
        } else if (0 == size) {
            return 0;
        }
        cout << std::dec << "add start." << "old size():" << this->size() << ",size:" << size << ",_capacity:" << _capacity << endl;
        if ((this->size() + size) > _capacity) {
            return -1;
        }

        if (_begin <= _end) {
            if ((_end + size) < _capacity) {
                memcpy(_data_buf + _end, src, size);
                _end += size;
            }
            else {
                uint32_t end_length = _capacity - _end;
                memcpy(_data_buf + _end, src, end_length);
                memcpy(_data_buf, src + end_length, size - end_length);
                _end = size - end_length;
            }
        }
        else {
            memcpy(_data_buf + _end, src, size);
            _end += size;
        }
        cout << std::dec << "begin:" << _begin << ",end:" << _end << ",old size():" << this->size()<< ",capacity:" << _capacity << endl;
        return 0;
    }

    int32_t take(int offset, uint8_t * dst, int size) {
        // printf("\nbegin %d, offset %d, size %d, thissize %d, capacity %d\n", _begin, offset, size, this->size(), _capacity);
        if (nullptr == dst) {
            return -1;
        } else if (0 == size) {
            return 0;
        }

        if ((offset + size) > this->size()) {
            return -1;
        }

        if ((_begin + offset + size) < _capacity) {
            memcpy(dst, _data_buf + _begin + offset, size);
        }
        else if ((_begin + offset) < _capacity) {
            uint32_t temp_length = _capacity - _begin - offset;
            memcpy(dst, _data_buf + _begin + offset, temp_length);
            memcpy(dst + temp_length, _data_buf, size - temp_length);
        }
        else {
            memcpy(dst, _data_buf + _begin + offset - _capacity, size);
        }

        return 0;
    }

    int32_t take(int offset, std::string &dst, int size) {
        if ((offset + size) > this->size()) {
            cout << std::dec << "take over." << "offset:"<< offset << ",size:" << size << ",old_size:" << this->size() << endl;
            return -1;
        }

        if ((_begin + offset + size) < _capacity) {
            dst.assign((char *)(_data_buf + _begin + offset), size);
        }
        else if ((_begin + offset) < _capacity) {
            uint32_t temp_length = _capacity - _begin - offset;
            dst.assign((char *)(_data_buf + _begin + offset), temp_length);
            dst.append((char *)(_data_buf), size - temp_length);
        }
        else {
            dst.assign((char *)(_data_buf + _begin + offset - _capacity), size);
        }

        return 0;
    }

    int32_t discard(uint32_t offset, uint32_t size) {
        if ((offset + size) > this->size()) {
            return -1;
        }

        cout << dec << "discard start. begin:"  << _begin << ",offset:" << offset << ",size:" << size << ",_capacity:" << _capacity << endl;

        if ((_begin + offset + size) < _capacity) {
            _begin = _begin + offset + size;
            cout << dec << "1. _begin:" << _begin << ",offset:" << offset << ",size:" << size << ",_capacity:" << _capacity << endl;
        }
        else {
            uint32_t temp_length = _capacity - _begin - offset;
            _begin = size - temp_length;
            cout << dec << "2. _begin:" << _begin << ",offset:" << offset << ",size:" << size << ",_capacity:" << _capacity << endl;
        }

        cout << std::dec << "discard end. _begin:" << _begin << ",offset:"<< offset 
            << ",size:" << size << ",old_size:" << this->size() << ",_end:" << _end << endl;
        return 0;
    }

    uint8_t &operator[](uint32_t i) {
        return at(i);
    }

    uint8_t &at(uint32_t i) {
        if ((_begin + i) < _capacity) {
            return _data_buf[_begin + i];
        }
        else {
            return _data_buf[i - (_capacity - _begin)];
        }
    }

    uint32_t size() {
        if (_end >= _begin) {
            return _end - _begin;
        }
        else {
            return _capacity - _begin + _end;
        }
    }

    void reset() {
        _begin = 0;
        _end = 0;
    }

    void print_pool(){
        
        cout << std::dec << "begin:" << _begin << ",end:" << _end << ",capacity:" << _capacity << endl;
        int length = _end - _begin;
        for (int i = 0; i < length; i++) {
        cout << std::uppercase << std::setfill('0') << std::setw(2)
            << std::hex << ((*(_data_buf + _begin + i)) & 0xff) << " ";
        }
        cout << endl;
    }

protected:
    uint8_t * _data_buf;
    uint32_t _begin;
    uint32_t _end;
    uint32_t _capacity;
};

bool find_header(uint32_t offset, uint32_t &index, uint32_t &length, DataPool& _data_pool) {
    cout << dec << "find_header start. offset:" << offset << ",index:" << index << ",length:" << length << endl;
    uint32_t i = offset;

    if (_data_pool.size() < 3) {
        return false;
    }

    for (i = offset; i < (_data_pool.size() - 4); ++i) {
        if ((_data_pool[i] == 0x00) 
            && (_data_pool[i + 1] == 0x00)
            && (_data_pool[i + 2] == 0x00)
            && (_data_pool[i + 3] == 0x01)) {
            index = i;
            length = 4;
            cout << dec << "find_header end. offset:" << offset << ",index:" << index << ",length:" << length << endl;
            return true;
        }
        
        if ((_data_pool[i] == 0x00) 
            && (_data_pool[i + 1] == 0x00)
            && (_data_pool[i + 2] == 0x01)) {
            index = i;
            length = 3;
            return true;
        }
    }

    if ((_data_pool[i] == 0x00) 
        && (_data_pool[i + 1] == 0x00)
        && (_data_pool[i + 2] == 0x01)) {
        index = i;
        length = 3;
        return true;
    }

    return false;
}

void print_int8(uint8_t* data, int length)
{
    for (int i = 0; i < length; i++) {
        cout << std::uppercase << std::setfill('0') << std::setw(2)
            << std::hex << ((*(data + i)) & 0xff) << " ";
        }
    cout << endl;
}

int process(DataPool& pool,  uint32_t begin, uint32_t end)
{
    uint32_t nal_length = end - begin;
    uint8_t data[1000];
    cout << dec << "nal_length:" << nal_length << ",begin:"<< begin << ",end:" << end << endl;

    if(nal_length < 20 ){
        pool.take(begin, data, nal_length);
        print_int8(data, nal_length);
    }else{
        uint32_t bytes_used = 0;
        int count = 0 ;
        //cout << dec << "left_length:" << (end - (begin + bytes_used)) << ",count:" << count++ << endl;
        while ((end - (begin + bytes_used)) > 20) {
            memset(data,0,1000);
            pool.take(begin + bytes_used, &data[0], 20);
            bytes_used += 20;
            print_int8(data, 20);
            //cout << dec << "left_length:" << (end - (begin + bytes_used)) << ",count:" << count++ << endl;
            count++;
        }

        memset(data,0,1000);
        int left = end - (begin + bytes_used);
        pool.take(begin + bytes_used, data, left);
        bytes_used += left;
        print_int8(data, left);
        cout << "take success. count:" << count << endl;
    }
    return 0;
}

int push_data(DataPool& pool)
{
    bool is_found = false;
    uint32_t begin_index = 0;
    uint32_t begin_header_length = 0;
    uint32_t end_index = 0;
    uint32_t end_header_length = 0;
    bool is_continued = true;

    while (is_continued) {
        cout << "=====================" << endl;
        is_found = find_header(0, begin_index, begin_header_length, pool);
        cout << dec << "is_found:" << is_found << ",begin_index:" << begin_index << ",begin_header_length:" << begin_header_length << endl;
        cout << "-------------------" << endl;
        is_found = find_header(begin_index + begin_header_length, end_index, end_header_length, pool);
        if( !is_found){
            cout << "not found header." << endl;
            break;
        }
        cout << dec << "is_found:" << is_found << ",end_index:" << end_index << ",end_header_length:" << end_header_length << endl;
        cout << "-------------------" << endl;

        process(pool, begin_index + begin_header_length, end_index);

        uint32_t nal_length = end_index - begin_index;
        pool.discard(begin_index, nal_length);
        sleep(2);
    }
    return 0;
}
int main() {
    DataPool pool(450);
    std::string str;

    // 打开H264文件
    ifstream file("test.H264", ios::binary);

    // 检查文件是否打开成功
    if (!file.is_open()) {
        cout << "Failed to open file!" << endl;
        return -1;
    }

    // 获取文件大小
    file.seekg(0, file.end);
    int size = file.tellg();
    file.seekg(0, file.beg);
    cout << "File size: " << size << endl;

    // 分配足够大小的内存存储文件内容
    char* buffer = new char[size];
    file.read(buffer, size);

    cout << "File content:" << endl;
    for (int i = 0; i < size; i++) {
        cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(buffer[i])) << " ";

    }
    cout << endl;


    pool.add((uint8_t*)buffer, size);
    pool.print_pool();

    push_data(pool);
    

   //继续打开H264文件
    ifstream file1("test1.H264", ios::binary);

    // 检查文件是否打开成功
    if (!file1.is_open()) {
        cout << "Failed to open file!" << endl;
        return -1;
    }

    // 获取文件大小
    file1.seekg(0, file1.end);
    size = file1.tellg();
    file1.seekg(0, file1.beg);
    cout << "File size: " << size << endl;

    // 分配足够大小的内存存储文件内容
    char* buffer1 = new char[size];
    file1.read(buffer1, size);

    cout << "File content:" << endl;
    for (int i = 0; i < size; i++) {
        cout << hex << setw(2) << setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(buffer[i])) << " ";

    }
    cout << endl;


    pool.add((uint8_t*)buffer1, size);
    pool.print_pool();

    push_data(pool);

    return 0;
}
