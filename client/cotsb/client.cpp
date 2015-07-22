#include "client.h"

#include <iostream>

namespace cotsb
{
    Client::Client() :
        _port(8888),
        _hostname("127.0.0.1"),
        _state(Client::Idle)
    {
        _socket.setBlocking(false);
        _selector.add(_socket);
    }

    void Client::port(uint16_t value)
    {
        _port = value;
    }
    uint16_t Client::port() const
    {
        return _port;
    }

    void Client::hostname(const std::string &value)
    {
        _hostname = value;
    }
    std::string Client::hostname() const
    {
        return _hostname;
    }

    void Client::start_client()
    {
        if (_state == Idle)
        {
            _state = Connecting;
        }
    }

    void Client::check_network()
    {
        _new_data.clear();
        _socket.receive(_new_data);
    }

    sf::Packet &Client::new_data()
    {
        return _new_data;
    }
    sf::TcpSocket &Client::socket()
    {
        return _socket;
    }

    void Client::game_tick()
    {
        if (_state == Connecting)
        {
            auto connect_result = _socket.connect(_hostname, _port);
            if (connect_result == sf::Socket::Done)
            {
                _state = Connected;
            }
            else if (connect_result == sf::Socket::Error)
            {
                _state = Error;
            }
        }
        else if (_state == Connected)
        {
            check_network();
        }
    }

    Client::State Client::state() const
    {
        return _state;
    }
}
