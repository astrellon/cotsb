#pragma once

#include <string>

#include <SFML/Network.hpp>

namespace cotsb
{
    class GameObject;
    class Player;

    class MessageManager
    {
        public:
            enum Type : uint8_t
            {
                Server = 1,
                ServerWhisper = 2,
                Say = 3,
                Whisper = 4
            };

            static void say(const std::string &message, GameObject *from);
            static void server(const std::string &message);
            static void server_whisper(const std::string &message, sf::TcpSocket *to);
            static void whisper(const std::string &message, GameObject *from, Player *to);

            static void process_packet(sf::Packet &packet);
    };
}
