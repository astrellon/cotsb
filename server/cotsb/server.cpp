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
#include <pthread.h> //for threading , link with lpthread

namespace cotsb
{
    Connection::Connection(int socket) :
        _socket(socket)
    {
        _listen_thread = std::thread([] (Connection *connection)
        {
            connection->init_handler();
        }, this);
        _write_thread = std::thread([] (Connection *connection)
        {

        }, this);
    }

    void Connection::init_handler()
    {
        //Get the socket descriptor
        int read_size;
        char client_message[2000];
        client_message[0] = '\0';

        //Send some messages to the client
        write_str("Greetings! I am your connection handler\n");
        write_str("Now type something and i shall repeat what you type \n");

        //Receive a message from client
        while( (read_size = recv(_socket , client_message , 2000 , 0)) > 0 )
        {
            //Send the message back to client
            client_message[read_size] = '\0';
            printf("Received >%s<\n", client_message);
            write(_socket , client_message , strlen(client_message));
        }

        if(read_size == 0)
        {
            puts("Client disconnected");
            fflush(stdout);
        }
        else if(read_size == -1)
        {
            perror("recv failed");
        }
    }

    void Connection::write_str(const std::string &str)
    {
        write(_socket , str.c_str(), str.size());
    }

    Server::Server()
    {
    }

    int Server::start_server()
    {
        int socket_desc , client_sock , c , new_sock;
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
        server.sin_port = htons( 8888 );

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

            pthread_t sniffer_thread;
            new_sock = client_sock;

            auto connection = new Connection(new_sock);
            _connections.push_back(std::unique_ptr<Connection>(connection));
            /*
            if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) &new_sock) < 0)
            {
                perror("could not create thread");
                return 1;
            }
            */

            //Now join the thread , so that we dont terminate before the thread
            //pthread_join( sniffer_thread , NULL);
            puts("Handler assigned");
        }

        if (client_sock < 0)
        {
            perror("accept failed");
            return 1;
        }

        return 0;

    }
}

/*
 * This will handle connection for each client
 * */
