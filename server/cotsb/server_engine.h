#pragma once

#include "server.h"

namespace cotsb
{
    class ServerEngine
    {
        public:
            static bool init();

            static void shutdown();
            static bool is_running();

            static void server_loop();

        private:
            static Server s_server;
            static bool s_running;
    };
}
