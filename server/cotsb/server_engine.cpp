#include "server_engine.h"

#include <cotsb/logging.h>
#include "map.h"

namespace cotsb
{
    Server ServerEngine::s_server;
    bool ServerEngine::s_running = false;

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

        auto map1 = new Map("map1", 3, 3);
        MapManager::map(map1);
        map1->tile(0, 0, "wall");
        map1->tile(1, 0, "wall");
        map1->tile(2, 0, "wall");
        
        map1->tile(0, 1, "grass");
        map1->tile(1, 1, "wall");
        map1->tile(2, 1, "wall");
        
        map1->tile(0, 2, "grass");
        map1->tile(1, 2, "grass");
        map1->tile(2, 2, "wall");
    
        logger % "Info" << "Started server" <<endl;

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
        if (command == Commands::LOAD_MAP)
        {
            std::string map_name;
            packet >> map_name;

            logger % "Info" << "Request for map: " << map_name << endl;
        }
        else if (command == Commands::MESSAGE)
        {
            std::string message;
            packet >> message;

            sf::Packet echo_message;
            echo_message << static_cast<uint16_t>(command) << message;
            s_server.broadcast(echo_message, socket);
        }
    }
}
