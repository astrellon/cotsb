#include "client.h"

#include <cotsb/logging.h>

namespace cotsb
{
    // Response {{{
    Client::Response::Response(sf::Packet *data) :  
        _data(data),
        _command(Commands::Unknown),
        _id(0u),
        _success(false)
    {
        auto &input = *data;
        uint16_t command_temp;
        input >> command_temp;
        _command = static_cast<Commands::Type>(command_temp);

        input >> _id;
        input >> _success;

        if (!_success)
        {
            input >> _error_message;
        }
    }

    sf::Packet &Client::Response::data()
    {
        return *_data;
    }
    Commands::Type Client::Response::command() const
    {
        return _command;
    }
    uint32_t Client::Response::id() const
    {
        return _id;
    }
    bool Client::Response::success() const
    {
        return _success;
    }
    const std::string &Client::Response::error_message() const
    {
        return _error_message;
    }
    // }}}
    
    // Request {{{
    uint32_t Client::Request::s_id_counter = 0u;

    Client::Request::Request(Commands::Type command) :
        _id(0u),
        _command(command)
    {
        _data << static_cast<uint16_t>(command) << _id; 
    }
    Client::Request::Request(Commands::Type command, Client::ResponseHandler handler) :
        _id(0u),
        _command(command),
        _handler(handler)
    {
        _id = ++s_id_counter;
        _data << static_cast<uint16_t>(command) << _id; 
    }

    uint32_t Client::Request::id() const
    {
        return _id;
    }
    Commands::Type Client::Request::command() const 
    {
        return _command;
    }
    Client::ResponseHandler Client::Request::handler() const
    {
        return _handler;
    }
    sf::Packet &Client::Request::data()
    {
        return _data;
    }
    // }}}
    
    // Client {{{
    Client::Client() :
        _port(8888),
        _hostname("localhost"),
        _state(Client::Idle),
        _has_connected(false)
    {
        _socket.setBlocking(false);
        _pending_new_data = new sf::Packet();
    }
    Client::~Client()
    {
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
            _socket.send(iter->data());
            if (iter->handler() != nullptr)
            {
                _awaiting_responses[iter->id()] = iter;
            }
        }

        _to_send.clear();
    }

    Client::Request &Client::send(Commands::Type command)
    {
        auto new_request = new Request(command);
        _to_send.push_back(new_request);
        return *new_request;
    }
    Client::Request &Client::send(Commands::Type command, Client::ResponseHandler handler)
    {
        auto new_request = new Request(command, handler);
        _to_send.push_back(new_request);
        return *new_request;
    }

    Client::Request *Client::awaiting_response(uint32_t id) const
    {
        auto find = _awaiting_responses.find(id);
        if (find == _awaiting_responses.cend())
        {
            return find->second;
        }
        return nullptr;
    }
    void Client::remove_awaiting_response(uint32_t id)
    {
        auto find = _awaiting_responses.find(id);
        if (find != _awaiting_responses.end())
        {
            _awaiting_responses.erase(find);
        }
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
        }

        if (_state == Connecting)
        {
            auto connect_result = _socket.connect(_hostname, _port);
            if (connect_result == sf::Socket::Done)
            {
                _has_connected = true;
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
    // }}}
}
