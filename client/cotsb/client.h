#pragma once

#include <SFML/Network.hpp>

#include <stdint.h>
#include <vector>
#include <memory>
#include <functional>

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
                    uint32_t id() const;
                    bool success() const;
                    const std::string &error_message() const;

                private:
                    std::unique_ptr<sf::Packet> _data;
                    Commands::Type _command;
                    uint32_t _id;
                    bool _success;
                    std::string _error_message;
            };
            
            typedef std::function<void (Response *)> ResponseHandler;
            // }}}

            // Request {{{
            class Request
            {
                public: 
                    Request(Commands::Type command);
                    Request(Commands::Type command, ResponseHandler handler);

                    uint32_t id() const;
                    Commands::Type command() const;
                    ResponseHandler handler() const;
                    sf::Packet &data();
                
                private:
                    uint32_t _id;
                    Commands::Type _command;
                    ResponseHandler _handler;
                    sf::Packet _data;

                    static uint32_t s_id_counter;
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

            Request &send(Commands::Type command);
            Request &send(Commands::Type command, ResponseHandler handler);

            ResponseHandler response_handler(uint32_t id) const;
            void remove_response_handler(uint32_t id);

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

            ResponseList _new_data;
            sf::Packet *_pending_new_data;
            sf::TcpSocket _socket;
            std::vector<std::unique_ptr<Request> > _to_send;
            std::map<uint32_t, ResponseHandler > _response_handlers;

            void check_network();
            // }}}
    };
}
