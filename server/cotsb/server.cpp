/*
    C socket server example, handles multiple clients using threads
*/
 
#include "server.h"

#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <sstream>

namespace cotsb
{
    Stream::Stream()
    {

    }

    bool Stream::write(const uint8_t *data, size_t size)
    {
        if (data == nullptr || size == 0u)
        {
            return false;
        }

        std::lock_guard<std::mutex> lock_guard(_lock);
        for (auto i = 0u; i < size; i++)
        {
            _data.push_back(data[i]);
        }

        _cv.notify_one();
        return true;
    }
    bool Stream::write(const std::string &str)
    {
        std::cout << "Write: " << str << " to stream\n";
        return write((const uint8_t *)str.c_str(), str.size());
    }

    size_t Stream::read(std::ostream &output, size_t count)
    {
        std::unique_lock<std::mutex> lock_guard(_lock);
        _cv.wait(lock_guard, [this]()
        {
            return _data.size() > 0u;
        });

        auto max = count;
        if (max == 0u || max > _data.size())
        {
            max = _data.size();
        }

        std::cout << "Read: " << max << " bytes from stream\n";
        for (auto i = 0u; i < max; i++)
        {
            output << (char)_data[i];
        }
        _data.erase(_data.begin(), _data.begin() + max);

        return max;
    }
    size_t Stream::read(uint8_t *output, size_t count)
    {
        std::unique_lock<std::mutex> lock_guard(_lock);
        _cv.wait(lock_guard, [this]()
        {
            return _data.size() > 0u;
        });

        auto max = count;
        if (max > _data.size())
        {
            max = _data.size();
        }

        auto read_count = 0u;
        for (auto i = 0u; i < max; i++, read_count++)
        {
            output[i] = _data[read_count];
        }
        std::cout << "Read: " << max << " bytes from uint8\n";

        _data.erase(_data.begin(), _data.begin() + max);

        return max;
    }

    bool Stream::has_data() const
    {
        return _data.size() > 0u;
    }

    Connection::Connection(int socket) :
        _socket(socket),
        _connection_open(true)
    {
        _listen_thread = std::thread([] (Connection *connection)
        {
            connection->init_handler();
        }, this);
        _write_thread = std::thread([] (Connection *connection)
        {
            connection->write_handler();
        }, this);
    }

    Stream &Connection::incoming_data()
    {
        return _incoming_data;
    }
    Stream &Connection::outbound_data()
    {
        return _outgoing_data;
    }

    void Connection::init_handler()
    {
        //Get the socket descriptor
        int read_size;
        //char client_message[2000];
        std::array<uint8_t, 2048> buffer;

        //Send some messages to the client
        _outgoing_data.write("Greetings! I am your connection handler\n");
        _outgoing_data.write("Now type something and i shall repeat what you type \n");

        //Receive a message from client
        while ( (read_size = recv(_socket, buffer.data(), 2048 , 0)) > 0 )
        {
            _incoming_data.write(buffer.data(), read_size);
        }

        _connection_open = false;
        if (read_size == 0)
        {
            puts("Client disconnected");
            fflush(stdout);
        }
        else if (read_size == -1)
        {
            perror("recv failed");
        }
    }
    void Connection::write_handler()
    {
        std::array<uint8_t, 2048> buffer;
        while (_connection_open)
        {
            auto read_bytes = _outgoing_data.read(buffer.data(), 2048);
            std::cout << "Read: " << read_bytes << " to write to socket\n";
            write(_socket, buffer.data(), read_bytes);
        }
    }

    Server::Server() :
        _port(8888)
    {
    }

    void Server::port(int value)
    {
        _port = value;
    }
    int Server::port() const
    {
        return _port;
    }

    int Server::start_server()
    {
        _accept_connections = std::thread([] (Server *self)
        {
            int socket_desc , client_sock , c;
            struct sockaddr_in server , client;

            //Create socket
            socket_desc = socket(AF_INET , SOCK_STREAM , 0);
            if (socket_desc == -1)
            {
                printf("Could not create socket");
            }
            puts("Socket created");

            //Prepare the sockaddr_in structure
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = INADDR_ANY;
            server.sin_port = htons( self->port() );

            //Bind
            if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
            {
                //print the error message
                perror("bind failed. Error");
                return 1;
            }
            puts("bind done");

            //Listen
            listen(socket_desc , 3);

            //Accept and incoming connection
            puts("Waiting for incoming connections...");
            c = sizeof(struct sockaddr_in);

            while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
            {
                puts("Connection accepted");

                auto connection = new Connection(client_sock);
                self->_connections.push_back(std::unique_ptr<Connection>(connection));
                puts("Handler assigned");
            }

            if (client_sock < 0)
            {
                perror("accept failed");
                return 1;
            }

            return 0;
        }, this);

        while (true)
        {
            for (auto i = 0u; i < _connections.size(); i++)
            {
                auto connection = _connections[i].get();
                if (connection->incoming_data().has_data())
                {
                    std::stringstream input;
                    connection->incoming_data().read(input, 0u);
                    std::string result = input.str();

                    broadcast(result);
                }
            }

            // Sleep for 10ms for now.
            usleep(10000);
        }

        _accept_connections.join();

        return 0;
    }

    void Server::broadcast(const std::string &str)
    {
        std::cout << "Broadcasting: " << str << "\n";
        for (auto i = 0u; i < _connections.size(); i++)
        {
            _connections[i]->outbound_data().write(str);
        }
    }
}
