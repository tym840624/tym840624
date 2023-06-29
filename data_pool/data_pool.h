#pragma once 

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <stdio.h>

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

        return 0;
    }

    int32_t take(uint32_t offset, uint8_t * dst, uint32_t size) {
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

    int32_t take(uint32_t offset, std::string &dst, uint32_t size) {
        if ((offset + size) > this->size()) {
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

        if ((_begin + offset + size) < _capacity) {
            _begin = _begin + offset + size;
        }
        else {
            uint32_t temp_length = _capacity - _begin - offset;
            _begin = size - temp_length;
        }

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

protected:
    uint8_t * _data_buf;
    uint32_t _begin;
    uint32_t _end;
    uint32_t _capacity;
};
