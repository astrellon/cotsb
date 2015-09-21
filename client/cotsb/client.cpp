#include "client.h"

#include <cotsb/logging.h>
#include <functional>

namespace cotsb
{
    // Response {{{
    Client::Response::Response(sf::Packet *data) :  
        _data(data),
        _command(Commands::Unknown)
    {
        auto &input = *data;
        CommandType command_temp;
        input >> command_temp;
        _command = static_cast<Commands::Type>(command_temp);
    }

    sf::Packet &Client::Response::data()
    {
        return *_data;
    }
    Commands::Type Client::Response::command() const
    {
        return _command;
    }
    // }}}
    
    // Client {{{
    Client::Client() :
        _port(8888),
        _hostname("localhost"),
        _state(Client::Idle),
        _has_connected(false)
    {
        //_socket.setBlocking(false);
        _pending_new_data = new sf::Packet();
    }
    Client::~Client()
    {
        logger % "Info" << "Client over" << endl;
        if (_pending_new_data != nullptr)
        {
            delete _pending_new_data;
        }
    }

    void Client::port(uint16_t value)
    {
        _port = value;
    }
    uint16_t Client::port() const
    {
        return _port;
    }

    bool Client::has_connected() const
    {
        return _has_connected;
    }
    void Client::clear_has_connected()
    {
        _has_connected = false;
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
            auto response = new Response(_pending_new_data);
            _new_data.push_back(std::unique_ptr<Response>(response));

            _pending_new_data = new sf::Packet();
        }

        for (auto &iter : _to_send)
        {
            _socket.send(*iter);
        }

        _to_send.clear();
    }

    sf::Packet &Client::send(Commands::Type command)
    {
        auto new_request = new sf::Packet();
        (*new_request) << static_cast<CommandType>(command);
        _to_send.push_back(std::unique_ptr<sf::Packet>(new_request));
        return *new_request;
    }

    Client::ResponseList &Client::new_data()
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

            _connection_thread = new std::thread(&Client::connecting_thread, this);
            _connection_thread->detach();
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
    
    void Client::connecting_thread()
    {
        logger % "Network" << "Attemtping to connect to: " << _hostname << ":" << _port << endl;
        auto connect_result = _socket.connect(_hostname, _port);
        if (connect_result == sf::Socket::Done)
        {
            logger % "Network" << "Connected" << endl;
            _has_connected = true;
            _state = Client::Connected;
            _socket.setBlocking(false);
        }
        else if (connect_result == sf::Socket::Error)
        {
            logger % "Error" << "Error connecting" << endl;
            _state = Error;
        }
    }

    // }}}
}
