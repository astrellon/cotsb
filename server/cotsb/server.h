#pragma once

#include <thread>
#include <vector>
#include <stdint.h>

#include <cotsb/connection.h>

namespace cotsb
{
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
