#include "server_engine.h"

#include <SFML/Graphics.hpp>

#include <cotsb/logging.h>
#include "map.h"
#include "map_tcp_serialiser.h"
#include "player_tcp_serialiser.h"
#include "game_object_tcp_serialiser.h"
#include "game_object.h"
#include "tile.h"
#include "profile.h"

#include "map_lua_deserialiser.h"
#include <utils/lua_serialiser.h>

#include <signal.h>
#include <string.h>

namespace cotsb
{
    void kill_sig_handler(int, siginfo_t*, void*)
    {
        ServerEngine::shutdown();
    }

    bool ServerEngine::s_running = false;
    Server ServerEngine::s_server;

    bool ServerEngine::init()
    {
        if (s_running)
        {
            return true;
        }
        
        struct sigaction act;

        memset(&act, 0, sizeof(struct sigaction));
        sigemptyset(&act.sa_mask);

        act.sa_sigaction = kill_sig_handler;

        act.sa_flags = SA_SIGINFO;

        if (-1 == sigaction(SIGINT, &act, NULL))
        {
            logger % "Error" << "sigaction()" << endl;
            return false;
        }

        s_running = true;
        s_server.port(8888);

        try
        {
            s_server.start_server();
        }
        catch (...)
        {
            return false;
        }

        TileManager::init();
        MapManager::init();

        auto map1 = MapLuaDeserialiser::deserialise("data/maps/map1.lua");
        if (map1 == nullptr)
        {
            logger % "Error" << "Failed to load map1!" << endl;
        }
        else
        {
            MapManager::map(map1);
        }
    
        logger % "Info" << "Started server" <<endl;

        s_server.on_connect(on_connect);
        s_server.on_disconnect(on_disconnect);

        return true;
    }
    void ServerEngine::deinit()
    {
        ProfileManager::deinit();
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
        sf::Time sleep_time = sf::milliseconds(16);
        while (s_running)
        {
            s_server.check_network();

            for (const auto &iter : s_server.new_data())
            {
                auto &packet = *iter.second;
                CommandType command_temp;
                packet >> command_temp;

                process_command(iter.first, static_cast<Commands::Type>(command_temp), packet); 
            }
            s_server.clear_new_data();

            update(sleep_time.asSeconds());
            sf::sleep(sleep_time);
        }
    }
            
    sf::Packet &ServerEngine::send(Commands::Type command, sf::TcpSocket *socket)
    {
        return s_server.send(command, socket);
    }
    sf::Packet &ServerEngine::send_callback(Commands::Type command, Packet::PacketSendCallback callback)
    {
        return s_server.send_callback(command, callback);
    }
    sf::Packet &ServerEngine::broadcast(Commands::Type command, sf::TcpSocket *skip_socket)
    {
        return s_server.broadcast(command, skip_socket);
    }

    void ServerEngine::process_command(sf::TcpSocket *socket, Commands::Type command, sf::Packet &packet)
    {
        if (command == Commands::LoadMap)
        {
            std::string map_name;
            packet >> map_name;
                
            auto player = PlayerManager::player(socket);

            logger % "Info" << "Request for map: " << map_name << endl;
            auto &response = s_server.send(Commands::NewMap, socket); 
            response << map_name;

            auto found_map = MapManager::map(map_name);
            if (found_map == nullptr)
            {
                response << false << "Failed to find map.";
            }
            else
            {
                response << true;
                MapTcpSerialiser::serialise(*found_map, response);

                if (player->current_map() == found_map)
                {
                    player->state(Player::LoadingMap);
                }

                for (auto &obj : found_map->game_objects())
                {
                    send_game_obj(obj, player);
                }
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
            std::string profile_name;
            packet >> profile_name;

            auto profile = ProfileManager::find_load_profile(profile_name);
            if (profile == nullptr)
            {
                profile = ProfileManager::create_profile(profile_name);
                setup_profile(profile);
                ProfileManager::save_profile(profile);
            }

            auto player = PlayerManager::create_player(socket);
            profile->player(player);
            player->profile(profile);

            auto player_game_object = GameObjectManager::create_game_object<GameObject>();
            player->game_object(player_game_object);
            player_game_object->setPosition(profile->location());
            player_game_object->colour(profile->colour());
            // Starter map
            auto map = MapManager::map(profile->map_name());
            player_game_object->current_map(map);
            // The player knows what map it's on, but it's added to that map until
            // the client says that it's ready.
            player->current_map(map);
            map->add_game_object(player_game_object);

            logger % "Network" << "Player joined " << profile->display_name() << endl;
            
            // Tell everyone except the original player about the new player
            auto &broadcast_player = s_server.broadcast(Commands::NewPlayer, socket);
            PlayerTcpSerialiser::serialise(*player, broadcast_player);
            
            // Tell everyone about the new game object for the player
            broadcast_game_obj(player_game_object);
            
            // Respond directly to the player about the player info.
            auto &response = s_server.send(Commands::JoinedGame, socket);
            PlayerTcpSerialiser::serialise(*player, response);
        }
        else if (command == Commands::LoadedPlayerMap)
        {
            auto player = PlayerManager::player(socket);
            if (player->current_map() == nullptr)
            {
                logger % "Error" << "Cannot add player to null map!" << endl;
                return;
            }

            player->state(Player::Ready);
            player->current_map()->add_player(player);
        }
        else if (command == Commands::MoveInDirection)
        {
            auto player = PlayerManager::player(socket);
            int8_t x, y;
            packet >> x >> y;
            player->move(x, y);

            logger % "Info" << "Move in direction: " << static_cast<int32_t>(x) << ", " << static_cast<int32_t>(y) << endl;
        }
        else if (command == Commands::CreateProfile)
        {
            std::string profile_name;
            std::string display_name;
            packet >> profile_name >> display_name;
        }
    }

    void ServerEngine::broadcast_game_obj(GameObject *obj)
    {
        for (auto &player : PlayerManager::players())
        {
            send_game_obj(obj, player.second.get());
        }
    }
    void ServerEngine::send_game_obj(GameObject *obj, Player *player)
    {
        auto id = obj->id();
        if (!player->has_seen_game_obj(id))
        {
            player->add_seen_game_obj(id);

            auto &send_game_obj = s_server.send(Commands::NewGameObject, player->socket());
            GameObjectTcpSerialiser::serialise(obj, send_game_obj);
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
        ProfileManager::save_profile(player->profile());

        PlayerManager::remove_player(socket);
    }

    void ServerEngine::update(float dt)
    {
        MapManager::update(dt);
        PlayerManager::update(dt);
        GameObjectManager::check_for_updates();
    }

    void ServerEngine::setup_profile(Profile *profile)
    {
        profile->map_name("map1");
        profile->location(sf::Vector2f(1, 1));
        profile->colour(sf::Color::Cyan);
    }
}
