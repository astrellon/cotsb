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

#include <iostream>

namespace cotsb
{
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
