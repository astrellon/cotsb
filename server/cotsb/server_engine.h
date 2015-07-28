#pragma once

#include "server.h"

#include <cotsb/commands.h>

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

            static void process_command(sf::TcpSocket *socket, Commands::Type command, sf::Packet &packet);
    };
}
