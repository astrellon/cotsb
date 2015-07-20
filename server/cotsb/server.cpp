#include "server.h"

#include <iostream>

namespace cotsb
{
    Server::Server(uint16_t port) :
        _port(port)
    {

    }

    void Server::start_server()
    {
        _listener.listen(_port);
        _client_selector.add(_listener);
        _listener.setBlocking(false);

        _pending_socket = std::unique_ptr<sf::TcpSocket>(new sf::TcpSocket());
        _pending_packet = std::unique_ptr<sf::Packet>(new sf::Packet());
    }

    void Server::check_network()
    {
        // The listener is ready: there is a pending connection
        if (_listener.accept(*_pending_socket) == sf::Socket::Done)
        {
            auto client = _pending_socket.get();
            std::cout << "New client: " << client->getRemoteAddress() << ":" << client->getRemotePort() << "\n";
            client->setBlocking(false);
            // Add the new client to the clients list
            // Add the new client to the selector so that we will
            // be notified when he sends something
            _clients.push_back(std::move(_pending_socket));

            sf::Packet message;
            message << "Hi there";
            client->send(message);

            _pending_socket = std::unique_ptr<sf::TcpSocket>(new sf::TcpSocket());
        }
        
        // The listener socket is not ready, test all other sockets (the clients)
        //for (auto &iter : _clients)
        for (auto i = 0u; i < _clients.size(); i++)
        {
            // The client has sent some data, we can receive it
            auto client = _clients[i].get();
            auto result = client->receive(*_pending_packet); 
            if (result == sf::Socket::Done)
            {
                std::cout << "Received data\n";
                _new_data.push_back(ClientDataPair(client, std::move(_pending_packet)));
                _pending_packet = std::unique_ptr<sf::Packet>(new sf::Packet());
            }
            else if (result == sf::Socket::Disconnected)
            {
                std::cout << "Client disconnected\n";
                _clients.erase(_clients.begin() + i);
                i--;
                continue;
            }
        }
    }

    const Server::ClientDataList &Server::new_data() const
    {
        return _new_data;
    }
    void Server::clear_new_data()
    {
        _new_data.clear();
    }

    void Server::broadcast(sf::Packet &data)
    {
        for (auto &iter : _clients)
        {
            iter->send(data);
        }
    }
}
