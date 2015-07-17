#include "client.h"
/*
    C ECHO client example using sockets
*/
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <sstream>

namespace cotsb
{
    Client::Client(int port) : 
        _port(port)
    {

    }

    void Client::start_client()
    {
        int sock;
        struct sockaddr_in server;
        //char message[1000] , server_reply[2000];

        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            printf("Could not create socket");
        }
        puts("Socket created");

        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_family = AF_INET;
        server.sin_port = htons( _port );

        //Connect to remote server
        if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
        {
            perror("connect failed. Error");
            return;
        }

        puts("Connected\n");
        _connection.start(sock);

        /*
        //Receive a reply from the server
        int read_size = 0;
        if( (read_size = recv(sock , server_reply , 2000 , 0)) < 0)
        {
            puts("recv failed");
            return;
        }

        server_reply[read_size] = '\0';
        puts("Connection message:");
        puts(server_reply);

        //keep communicating with server
        while(1)
        {
            printf("Enter message : ");
            scanf("%s" , message);

            //Send some data
            printf("Sending data: >%s<\n", message);
            if( send(sock , message , strlen(message) , 0) < 0)
            {
                puts("Send failed");
                return;
            }

            //Receive a reply from the server
            int read_size = 0;
            if( (read_size = recv(sock , server_reply , 1999 , 0)) < 0)
            {
                puts("recv failed");
                break;
            }

            server_reply[read_size] = '\0';
            puts("Server reply :");
            puts(server_reply);

        }
        */

        std::thread read_thead([] (Connection *connection)
        {
            while (connection->is_open())
            {
                if (connection->incoming_data().has_data())
                {
                    std::stringstream input;
                    connection->incoming_data().read(input, 0u);
                    std::string result = input.str();

                    std::cout << "Received data: >" << result << "<\n";
                }

                usleep(10000);
            }
        }, &_connection);

        while (true)
        {
            //printf("Enter message : ");
            //scanf("%s" , message);
            
            //printf("Sending data: >%s<\n", message);
            std::cout << "Enter message: ";
            std::string message;
            std::cin >> message;
            std::cout << "Sending data: " << message << "\n";
            _connection.outbound_data().write(message);
        }

        close(sock);

    }
}
