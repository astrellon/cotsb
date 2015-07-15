#pragma once

#include <thread>
#include <memory>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <condition_variable>

namespace cotsb
{
    class Stream
    {
        public:
            Stream();

            bool write(const uint8_t *data, size_t size);
            bool write(const std::string &data);

            size_t read(std::ostream &output, size_t);
            size_t read(std::vector<uint8_t> &output, size_t count);

        private:
            std::vector<uint8_t> _data;
            std::condition_variable _cv;
            std::mutex _lock;
    };

    class Connection
    {
        public:
            Connection(int socket);

            Stream &incoming_data();
            Stream &outboind_data();

        private:
            std::thread _listen_thread;
            std::thread _write_thread;
            Stream _incoming_data;
            Stream _outgoing_data;

            int _socket;

            void init_handler();
            void write_handler();
            void write_str(const std::string &str);
    };

    class Server
    {
        public:
            Server();

            int start_server();

        private:
            std::vector<std::unique_ptr<Connection> > _connections;
    };
}
