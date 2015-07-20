#include "client.h"

#include <iostream>

namespace cotsb
{
    Client::Client(uint16_t port) :
        _port(port)
    {
        _socket.setBlocking(false);
        _selector.add(_socket);
    }

    sf::Socket::Status Client::start_client()
    {
        return _socket.connect("127.0.0.1", _port);
    }

    void Client::check_network()
    {
        _new_data.clear();
        auto receive_result = _socket.receive(_new_data);
    }

    sf::Packet &Client::new_data()
    {
        return _new_data;
    }
    sf::TcpSocket &Client::socket()
    {
        return _socket;
    }
}
