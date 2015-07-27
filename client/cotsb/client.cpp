#include "client.h"

#include <cotsb/logging.h>

namespace cotsb
{
    Client::Client() :
        _port(8888),
        _hostname("127.0.0.1"),
        _state(Client::Idle)
    {
        _socket.setBlocking(false);
        _selector.add(_socket);
        _pending_new_data = std::unique_ptr<sf::Packet>(new sf::Packet());
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
            _state = PreConnecting;
        }
    }

    void Client::check_network()
    {
        _new_data.clear();
        while (_socket.receive(*_pending_new_data) == sf::Socket::Done)
        {
            _new_data.push_back(std::move(_pending_new_data));
            _pending_new_data = std::unique_ptr<sf::Packet>(new sf::Packet());
        }

        for (auto &iter : _to_send)
        {
            _socket.send(*iter.get());
        }
        _to_send.clear();
    }

    sf::Packet &Client::send(uint16_t command)
    {
        auto new_packet = new sf::Packet();
        *new_packet << command;
        _to_send.push_back(std::unique_ptr<sf::Packet>(new_packet));
        return *new_packet;
    }

    Client::PacketList &Client::new_data()
    {
        return _new_data;
    }
    sf::TcpSocket &Client::socket()
    {
        return _socket;
    }

    void Client::game_tick()
    {
        if (_state == PreConnecting)
        {
            logger % "Info" << "Connecting to server " << _hostname << ":" << _port << endl;
            _state = Connecting;
        }

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
