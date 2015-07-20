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
            Client(uint16_t port);

            sf::Socket::Status start_client();
            void check_network();

            sf::Packet &new_data();
            sf::TcpSocket &socket();

        private:
            uint16_t _port;

            sf::Packet _new_data;
            sf::TcpSocket _socket;
            sf::SocketSelector _selector;
    };
}
