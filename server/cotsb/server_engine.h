#pragma once

#include "server.h"
#include "player.h"

#include <map>
#include <memory>

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
            static bool s_running;
            static Server s_server;
            static PlayerManager s_players;

            static void process_command(sf::TcpSocket *socket, Commands::Type command, sf::Packet &packet);

            static void on_connect(sf::TcpSocket *socket);
            static void on_disconnect(sf::TcpSocket *socket);
    };
}
