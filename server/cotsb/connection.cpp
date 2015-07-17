#include "connection.h"

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>    //write

namespace cotsb
{
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
}
