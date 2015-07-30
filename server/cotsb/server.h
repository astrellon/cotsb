#pragma once

#include <SFML/Network.hpp>

#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <thread>
#include <mutex>

#include <cotsb/commands.h>

namespace cotsb
{
    class Server
    {
        public:
            Server(uint16_t port = 8888);

            void port(uint16_t value);
            uint16_t port() const;

            void start_server();
            void check_network();

            void broadcast(sf::Packet &data, sf::TcpSocket *skip_socket = nullptr);

            typedef std::pair<sf::TcpSocket *, std::unique_ptr<sf::Packet> > ClientDataPair;
            typedef std::vector<ClientDataPair> ClientDataList;

            const ClientDataList &new_data() const;
            void clear_new_data();

            sf::Packet &send(sf::TcpSocket *socket, Commands::Type command);

        private:
            uint16_t _port;

            std::vector<std::unique_ptr<sf::TcpSocket> > _clients;
            sf::SocketSelector _client_selector;
            sf::TcpListener _listener;
            ClientDataList _new_data;

            std::unique_ptr<sf::TcpSocket> _pending_socket;
            std::unique_ptr<sf::Packet> _pending_packet;
            
            typedef std::pair<sf::TcpSocket *, std::unique_ptr<sf::Packet> > SocketDataPair;
            std::vector<SocketDataPair> _to_send;
    };
}
