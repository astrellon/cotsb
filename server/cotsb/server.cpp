#include "server.h"

#include <iostream>

namespace cotsb
{
    Server::Server(uint16_t port) :
        _port(port),
        _running(false)
    {

    }

    void Server::start_server()
    {
        _listener.listen(_port);
        _client_selector.add(_listener);
        _listener.setBlocking(false);
        _running = true;
    }

    void Server::check_network()
    {
        // Make the selector wait for data on any socket
        if (_client_selector.wait())
        {
            // Test the listener
            if (_client_selector.isReady(_listener))
            {
                // The listener is ready: there is a pending connection
                auto client = new sf::TcpSocket;
                if (_listener.accept(*client) == sf::Socket::Done)
                {
                    client->setBlocking(false);
                    // Add the new client to the clients list
                    _clients.push_back(std::unique_ptr<sf::TcpSocket>(client));
                    // Add the new client to the selector so that we will
                    // be notified when he sends something
                    _client_selector.add(*client);
                }
                else
                {
                    // Error, we won't get a new connection, delete the socket
                    delete client;
                }
            }
            else
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
                            _new_data.push_back(ClientDataPair(iter.get(), std::unique_ptr<sf::Packet>(packet)));
                        }
                    }
                }
            }
        }
        else
        {
            std::cout << "No socket activity\n";
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
