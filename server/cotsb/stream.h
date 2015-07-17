#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <condition_variable>
#include <mutex>

namespace cotsb
{
    class Stream
    {
        public:
            Stream();

            bool write(const uint8_t *data, size_t size);
            bool write(const std::string &data);
            
            size_t read(std::ostream &output, size_t count);
            size_t read(uint8_t *output, size_t count);

            bool has_data() const;

        private:
            std::vector<uint8_t> _data;
            std::condition_variable _cv;
            std::mutex _lock;
    };
}
