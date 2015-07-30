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

            bool has_connected() const;
            void clear_has_connected();

            void hostname(const std::string &name);
            std::string hostname() const;

            void start_client();

            typedef std::vector<std::unique_ptr<sf::Packet> > PacketList;
            PacketList &new_data();

            sf::Packet &send(uint16_t command);

            sf::TcpSocket &socket();

            void game_tick();

            enum State
            {
                Idle,
                PreConnecting,
                Connecting,
                Connected,
                Error
            };

            State state() const;

        private:
            uint16_t _port;
            std::string _hostname;
            State _state;
            bool _has_connected;

            PacketList _new_data;
            std::unique_ptr<sf::Packet> _pending_new_data;
            sf::TcpSocket _socket;
            sf::SocketSelector _selector;
            std::vector<std::unique_ptr<sf::Packet> > _to_send;

            void check_network();
    };
}
