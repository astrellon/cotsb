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
            
            size_t read(std::ostream &output, size_t count);
            size_t read(uint8_t *output, size_t count);

            bool has_data() const;

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
            Stream &outbound_data();

        private:
            std::thread _listen_thread;
            std::thread _write_thread;
            Stream _incoming_data;
            Stream _outgoing_data;

            int _socket;
            bool _connection_open;

            void init_handler();
            void write_handler();
    };

    class Server
    {
        public:
            Server();

            void port(int value);
            int port() const;

            int start_server();

            void broadcast(const std::string &str);

        private:
            int _port;
            std::vector<std::unique_ptr<Connection> > _connections;
            std::thread _accept_connections;
    };
}
