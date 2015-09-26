#include "message.h"

#include "map.h"
#include "player.h"
#include "game_object.h"
#include "server_engine.h"

#include <cotsb/logging.h>

namespace cotsb
{
    void MessageManager::say(const std::string &message, GameObject *from)
    {
        auto &message_packet = ServerEngine::send_callback(Commands::Message,
            [from] (Packet &packet)
            {
                for (auto &player : from->current_map()->players())
                {
                    player->socket()->send(*packet.data());
                }
            });

        logger % "Say" << from->id() << " (" << from->current_map()->name() << ")" << ": " << message << endl;
        message_packet << static_cast<uint8_t>(Say) << from->id() << message;
    }
    void MessageManager::server(const std::string &message)
    {
        auto &message_packet = ServerEngine::broadcast(Commands::Message);
        message_packet << static_cast<uint8_t>(Server) << message;

        logger % "Server" << message << endl;
    }
    void MessageManager::server_whisper(const std::string &message, sf::TcpSocket *socket)
    {
        auto &message_packet = ServerEngine::send(Commands::Message, socket);
        message_packet << static_cast<uint8_t>(ServerWhisper) << message;
    }
    void MessageManager::whisper(const std::string &message, GameObject *from, Player *to)
    {
        auto &message_packet = ServerEngine::send(Commands::Message, to->socket());
        message_packet << static_cast<uint8_t>(Whisper) << from->id() << message;
    }

    void MessageManager::process_packet(sf::Packet &packet)
    {
        uint8_t type;
        uint32_t id;
        std::string message;

        packet >> type;
        Type message_type = static_cast<Type>(type);
        if (message_type == Say)
        {
            packet >> id >> message;
            auto game_obj = GameObjectManager::game_object(id);
            MessageManager::say(message, game_obj);
        }
        else if (message_type == Server)
        {
            packet >> message;
            MessageManager::server(message);
        }
        else if (message_type == Whisper)
        {
            packet >> id >> message;
            logger % "Whisper" << id << ": " << message << endl;
            //auto game_obj = GameObjectManager::game_object(id);
            //auto player = PlayerManager::player()
        }
    }
}
