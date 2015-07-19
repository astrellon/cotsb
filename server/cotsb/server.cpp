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
    }

    void Server::check_network()
    {
        //if (_client_selector.isReady(_listener))
        {
            //std::cout << "Listener has something\n";
            // The listener is ready: there is a pending connection
            auto client = new sf::TcpSocket;
            if (_listener.accept(*client) == sf::Socket::Done)
            {
                std::cout << "New client: " << client->getRemoteAddress() << ":" << client->getRemotePort() << "\n";
                client->setBlocking(false);
                // Add the new client to the clients list
                _clients.push_back(std::unique_ptr<sf::TcpSocket>(client));
                // Add the new client to the selector so that we will
                // be notified when he sends something
                _client_selector.add(*client);

                sf::Packet message;
                message << "Hi there";
                client->send(message);
            }
            else
            {
                // Error, we won't get a new connection, delete the socket
                //std::cout << "Error making client connection\n";
                delete client;
            }
        }
        //else
        {
            // The listener socket is not ready, test all other sockets (the clients)
            for (auto &iter : _clients)
            {
                auto& client = *iter;
                if (_client_selector.isReady(client))
                {
                    // The client has sent some data, we can receive it
                    auto packet = new sf::Packet;
                    if (client.receive(*packet) == sf::Socket::Done)
                    {
                        std::cout << "Received data\n";
                        _new_data.push_back(ClientDataPair(iter.get(), std::unique_ptr<sf::Packet>(packet)));
                    }
                }
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
