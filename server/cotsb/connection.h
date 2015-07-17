#pragma once

#include <thread>

#include "stream.h"

namespace cotsb
{
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

}
