#include "server_engine.h"

#include <cotsb/logging.h>
#include "map.h"
#include "map_tcp_serialiser.h"

namespace cotsb
{
    bool ServerEngine::s_running = false;
    Server ServerEngine::s_server;
    PlayerManager ServerEngine::s_players;

    bool ServerEngine::init()
    {
        if (s_running)
        {
            return true;
        }

        s_running = true;
        s_server.port(8888);
        s_server.start_server();

        MapManager::init();

        auto map1 = new Map("map1", 6, 5);
        MapManager::map(map1);
        map1->tile(0, 0, "wall");
        map1->tile(1, 0, "wall");
        map1->tile(2, 0, "wall");
        map1->tile(3, 0, "wall");
        map1->tile(4, 0, "wall");
        map1->tile(5, 0, "wall");
        
        map1->tile(0, 1, "grass");
        map1->tile(1, 1, "grass");
        map1->tile(2, 1, "water");
        map1->tile(3, 1, "water");
        map1->tile(4, 1, "water");
        map1->tile(5, 1, "wall");
        
        map1->tile(0, 2, "grass");
        map1->tile(1, 2, "grass");
        map1->tile(2, 2, "grass");
        map1->tile(3, 2, "grass");
        map1->tile(4, 2, "water");
        map1->tile(5, 2, "wall");
        
        map1->tile(0, 3, "grass");
        map1->tile(1, 3, "water");
        map1->tile(2, 3, "water");
        map1->tile(3, 3, "water");
        map1->tile(4, 3, "water");
        map1->tile(5, 3, "wall");
        
        map1->tile(0, 4, "grass");
        map1->tile(1, 4, "grass");
        map1->tile(2, 4, "water");
        map1->tile(3, 4, "water");
        map1->tile(4, 4, "water");
        map1->tile(5, 4, "wall");
    
        logger % "Info" << "Started server" <<endl;

        s_server.on_connect(on_connect);
        s_server.on_disconnect(on_disconnect);

        return true;
    }
    void ServerEngine::shutdown()
    {
        s_running = false;
    }
    bool ServerEngine::is_running()
    {
        return s_running;
    }

    void ServerEngine::server_loop()
    {
        sf::Time sleep_time = sf::milliseconds(1);
        while (s_running)
        {
            s_server.check_network();

            for (const auto &iter : s_server.new_data())
            {
                auto &packet = *iter.second;
                uint16_t command_temp;
                packet >> command_temp;

                process_command(iter.first, static_cast<Commands::Type>(command_temp), packet); 
            }
            s_server.clear_new_data();

            sf::sleep(sleep_time);
        }
    }

    void ServerEngine::process_command(sf::TcpSocket *socket, Commands::Type command, sf::Packet &packet)
    {
        if (command == Commands::LoadMap)
        {
            std::string map_name;
            packet >> map_name;

            logger % "Info" << "Request for map: " << map_name << endl;
            auto &response = s_server.send(Commands::NewMap, socket); 

            auto found_map = MapManager::map(map_name);
            if (found_map == nullptr)
            {
                response << false << "Failed to find map.";
            }
            else
            {
                response << true;
                MapTcpSerialiser::serialise(*found_map, response);
            }
        }
        else if (command == Commands::Message)
        {
            std::string message;
            packet >> message;
            
            auto &response = s_server.broadcast(command, socket);
            response << message;
        }
        else if (command == Commands::JoinGame)
        {
            std::string player_name;
            packet >> player_name;

            auto player = PlayerManager::create_player(socket);
            player->player_name(player_name);
            logger % "Network" << "Player joined " << player_name << endl;

            auto &response = s_server.send(Commands::JoinedGame, socket);
            response << true;

            auto &broadcast = s_server.broadcast(Commands::NewPlayer, socket);
            broadcast << player_name;
        }
    }

    void ServerEngine::on_connect(sf::TcpSocket *socket)
    {
        auto &welcome = s_server.send(Commands::Message, socket);
        welcome << "Welcome friend";
    }
    void ServerEngine::on_disconnect(sf::TcpSocket *socket)
    {
        auto player = PlayerManager::player(socket);
        if (player == nullptr)
        {
            // Player was disconnected before a player structure was created.
            return;
        }
        auto &goodbye = s_server.broadcast(Commands::PlayerLeft);
        goodbye << player->player_name();
    }
}
