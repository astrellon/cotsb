#pragma once

#include <SFML/Network.hpp>

#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>

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

            typedef std::unique_ptr<sf::Packet> UniquePacket;
            typedef std::unique_ptr<sf::TcpSocket> UniqueSocket;
            typedef std::pair<sf::TcpSocket*, UniquePacket> SocketDataPair;
            typedef std::vector<SocketDataPair> SocketDataList;

            typedef std::function<void (sf::TcpSocket *)> SocketHandler;
            void on_connect(SocketHandler handler);
            void on_disconnect(SocketHandler handler);

            const SocketDataList &new_data() const;
            void clear_new_data();

            sf::Packet &send(Commands::Type command, sf::TcpSocket *socket);
            sf::Packet &broadcast(Commands::Type command, sf::TcpSocket *skip_socket = nullptr);

        private:
            uint16_t _port;

            class SendPacket 
            {
                public:
                    Commands::Type command;
                    sf::TcpSocket *socket;
                    bool is_broadcast;
                    UniquePacket data;

                    inline SendPacket(Commands::Type command, sf::TcpSocket *socket, 
                            bool is_broadcast) :
                        command(command),
                        socket(socket),
                        is_broadcast(is_broadcast),
                        data(UniquePacket(new sf::Packet()))
                    {
                        *data << static_cast<uint16_t>(command);
                    }
            };

            typedef std::vector<SendPacket> PacketList;

            std::vector< UniqueSocket > _clients;
            sf::SocketSelector _client_selector;
            sf::TcpListener _listener;
            SocketDataList _new_data;

            UniqueSocket _pending_socket;
            UniquePacket _pending_packet;
            
            PacketList _to_send;

            SocketHandler _on_connect;
            SocketHandler _on_disconnect;
    };
}
