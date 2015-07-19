#include <iostream>
#include <cotsb/server.h>
#include <thread>
#include <functional>
#include <sstream>
#include <unistd.h>

int main(int argc , char *argv[])
{
    cotsb::Server server(8888);
    server.start_server();

    std::cout << "Started server\n";

    while (true)
    {
        server.check_network();

        for (const auto &iter : server.new_data())
        {
            std::cout << "New data!\n";
        }
        server.clear_new_data();

        usleep(1000);

    }
    std::cout << "Done\n";
}
 
