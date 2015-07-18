#pragma once

#include <cotsb/connection.h>

namespace cotsb
{
    class Client
    {
        public:
            Client(int port);

            void start_client();

        private:
            int _port;

            Connection _connection;
    };
}
