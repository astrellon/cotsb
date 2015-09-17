#pragma once

#include "server.h"
#include "player.h"

#include <map>
#include <memory>

#include <cotsb/commands.h>

namespace cotsb
{
    class Profile;

    class ServerEngine
    {
        public:
            static bool init();
            static void deinit();

            static void shutdown();
            static bool is_running();

            static void server_loop();

            static sf::Packet &send(Commands::Type command, sf::TcpSocket *socket);
            static sf::Packet &send_callback(Commands::Type command, Packet::PacketSendCallback callback);
            static sf::Packet &broadcast(Commands::Type command, sf::TcpSocket *skip_socket = nullptr);

            static void broadcast_game_obj(GameObject *obj);
            static void send_game_obj(GameObject *obj, Player *player);

        private:
            static bool s_running;
            static Server s_server;

            static void process_command(sf::TcpSocket *socket, Commands::Type command, sf::Packet &packet);

            static void on_connect(sf::TcpSocket *socket);
            static void on_disconnect(sf::TcpSocket *socket);

            static void update(float dt);

            static void setup_profile(Profile *profile);
    };
}
