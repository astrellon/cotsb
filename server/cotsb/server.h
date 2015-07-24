#pragma once

#include <SFML/Network.hpp>

#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <thread>
#include <mutex>

#include "map.h"

namespace cotsb
{
    class Server
    {
        public:
            Server(uint16_t port);

            void start_server();
            void check_network();

            void broadcast(sf::Packet &data);

            typedef std::pair<sf::TcpSocket *, std::unique_ptr<sf::Packet> > ClientDataPair;
            typedef std::vector<ClientDataPair> ClientDataList;

            const ClientDataList &new_data() const;
            void clear_new_data();

        private:
            uint16_t _port;

            std::vector<std::unique_ptr<sf::TcpSocket> > _clients;
            sf::SocketSelector _client_selector;
            sf::TcpListener _listener;
            ClientDataList _new_data;

            std::unique_ptr<sf::TcpSocket> _pending_socket;
            std::unique_ptr<sf::Packet> _pending_packet;

            Map _map1;
    };
}
