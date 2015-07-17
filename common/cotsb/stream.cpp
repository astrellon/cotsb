#include "stream.h"

#include <iostream>

namespace cotsb
{
    Stream::Stream()
    {

    }

    bool Stream::write(const uint8_t *data, size_t size)
    {
        if (data == nullptr || size == 0u)
        {
            return false;
        }

        std::lock_guard<std::mutex> lock_guard(_lock);
        for (auto i = 0u; i < size; i++)
        {
            _data.push_back(data[i]);
        }

        _cv.notify_one();
        return true;
    }
    bool Stream::write(const std::string &str)
    {
        //std::cout << "Write: " << str << " to stream\n";
        return write((const uint8_t *)str.c_str(), str.size());
    }

    size_t Stream::read(std::ostream &output, size_t count)
    {
        std::unique_lock<std::mutex> lock_guard(_lock);
        _cv.wait(lock_guard, [this]()
        {
            return _data.size() > 0u;
        });

        auto max = count;
        if (max == 0u || max > _data.size())
        {
            max = _data.size();
        }

        //std::cout << "Read: " << max << " bytes from stream\n";
        for (auto i = 0u; i < max; i++)
        {
            output << (char)_data[i];
        }
        _data.erase(_data.begin(), _data.begin() + max);

        return max;
    }
    size_t Stream::read(uint8_t *output, size_t count)
    {
        std::unique_lock<std::mutex> lock_guard(_lock);
        _cv.wait(lock_guard, [this]()
        {
            return _data.size() > 0u;
        });

        auto max = count;
        if (max > _data.size())
        {
            max = _data.size();
        }

        auto read_count = 0u;
        for (auto i = 0u; i < max; i++, read_count++)
        {
            output[i] = _data[read_count];
        }
        //std::cout << "Read: " << max << " bytes from uint8\n";

        _data.erase(_data.begin(), _data.begin() + max);

        return max;
    }

    bool Stream::has_data() const
    {
        return _data.size() > 0u;
    }
}
