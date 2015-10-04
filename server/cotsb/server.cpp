#include "server.h"

#include <cotsb/logging.h>
#include <cotsb/commands.h>

#include <exception>

namespace cotsb
{
    Server::Server(uint16_t port) :
        _port(port)
    {
        
    }

    void Server::port(uint16_t value)
    {
        _port = value;
    }
    uint16_t Server::port() const
    {
        return _port;
    }

    void Server::start_server()
    {
        auto status = _listener.listen(_port);
        if (status != sf::TcpSocket::Done)
        {
            logger % "Error" << "Bind error: " << status << endl;
            throw std::runtime_error("Failed to bind to listener port");
        }
        _client_selector.add(_listener);
        _listener.setBlocking(false);

        _pending_socket = UniqueSocket(new sf::TcpSocket());
        _pending_packet = UniquePacket(new sf::Packet());
    }

    void Server::check_network()
    {
        // The listener is ready: there is a pending connection
        if (_listener.accept(*_pending_socket) == sf::Socket::Done)
        {
            auto client = _pending_socket.get();
            //logger % "Network" << "New client: " << client->getRemoteAddress().toString() << ":" << client->getRemotePort() << endl; 
            client->setBlocking(false);
            // Add the new client to the clients list
            // Add the new client to the selector so that we will
            // be notified when he sends something
            _clients.push_back(std::move(_pending_socket));

            logger % "Network" << "New connection" << endl;

            if (_on_connect)
            {
                _on_connect(client);
            }

            _pending_socket = UniqueSocket(new sf::TcpSocket());
        }
        
        // The listener socket is not ready, test all other sockets (the clients)
        for (auto i = 0u; i < _clients.size(); i++)
        {
            // The client has sent some data, we can receive it
            auto client = _clients[i].get();
        
            // TODO Check if we need to wrap this in a while loop checking for 
            //      not ready status.
            auto result = client->receive(*_pending_packet); 
            if (result == sf::Socket::Done)
            {
                logger % "Network" << "Received data " << _pending_packet->getDataSize() << endl; 
                _new_data.push_back(SocketDataPair(client, std::move(_pending_packet)));
                _pending_packet = UniquePacket(new sf::Packet());
            }
            else if (result == sf::Socket::Disconnected)
            {
                logger % "Network" << "Client disconnected" << endl; 
                if (_on_disconnect)
                {
                    _on_disconnect(client);
                }
                _clients.erase(_clients.begin() + i);
                i--;
                
                continue;
            }
        }

        for (auto &packet : _to_send)
        {
            if (packet.type() == Packet::Broadcast)
            {
                for (auto &client : _clients)
                {
                    if (client.get() == packet.socket())
                    {
                        continue;
                    }
                    client->send(*packet.data());
                }
            }
            else if (packet.type() == Packet::Callback)
            {
                packet.callback()(packet);
            }
            else
            {
                packet.socket()->send(*packet.data());
            }
        }
        _to_send.clear();
    }

    void Server::on_connect(SocketHandler handler)
    {
        _on_connect = handler;
    }
    void Server::on_disconnect(SocketHandler handler)
    {
        _on_disconnect = handler;
    }

    const Server::SocketDataList &Server::new_data() const
    {
        return _new_data;
    }
    void Server::clear_new_data()
    {
        _new_data.clear();
    }

    sf::Packet &Server::broadcast(Commands::Type command, sf::TcpSocket *skip_socket)
    {
        _to_send.emplace_back(command, skip_socket, Packet::Broadcast);
        return *_to_send.back().data();
    }
            
    sf::Packet &Server::send(Commands::Type command, sf::TcpSocket *socket)
    {
        _to_send.emplace_back(command, socket, Packet::Single);
        return *_to_send.back().data();
    }

    sf::Packet &Server::send_callback(Commands::Type command, Packet::PacketSendCallback callback)
    {
        _to_send.emplace_back(command, callback);
        return *_to_send.back().data();
    }
}
