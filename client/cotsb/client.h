#pragma once

#include <SFML/Network.hpp>

#include <stdint.h>
#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <atomic>

#include <cotsb/commands.h>

namespace cotsb
{
    class Client
    {
        public:

            // Response {{{
            class Response
            {
                public:
                    Response(sf::Packet *data);

                    sf::Packet &data();
                    Commands::Type command() const;

                private:
                    std::unique_ptr<sf::Packet> _data;
                    Commands::Type _command;
            };
            // }}}

            // Client {{{
            Client();
            ~Client();

            void port(uint16_t value);
            uint16_t port() const;

            bool has_connected() const;
            void clear_has_connected();

            void hostname(const std::string &name);
            std::string hostname() const;

            void start_client();

            typedef std::vector<std::unique_ptr<Response> > ResponseList;
            ResponseList &new_data();

            sf::Packet &send(Commands::Type command);

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
            std::atomic<State> _state;
            bool _has_connected;

            ResponseList _new_data;
            sf::Packet *_pending_new_data;
            sf::TcpSocket _socket;
            std::vector<std::unique_ptr<sf::Packet> > _to_send;

            void check_network();
            // }}}
    };
}
