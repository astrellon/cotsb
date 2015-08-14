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

            typedef std::map<std::string, std::unique_ptr<Player> > PlayerMap;
            static const PlayerMap &players();

        private:
            static Server s_server;
            static bool s_running;
            static PlayerMap s_players;

            static void process_command(sf::TcpSocket *socket, Commands::Type command, sf::Packet &packet);
    };
}
