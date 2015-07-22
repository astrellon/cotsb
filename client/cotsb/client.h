#pragma once

#include <SFML/Network.hpp>

#include <stdint.h>
#include <vector>
#include <memory>

namespace cotsb
{
    class Client
    {
        public:
            Client();

            void port(uint16_t value);
            uint16_t port() const;

            void hostname(const std::string &name);
            std::string hostname() const;

            void start_client();

            sf::Packet &new_data();
            sf::TcpSocket &socket();

            void game_tick();

            enum State
            {
                Idle,
                Connecting,
                Connected,
                Error
            };

            State state() const;

        private:
            uint16_t _port;
            std::string _hostname;
            State _state;

            sf::Packet _new_data;
            sf::TcpSocket _socket;
            sf::SocketSelector _selector;

            void check_network();
    };
}
