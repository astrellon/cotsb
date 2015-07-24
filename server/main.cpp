#include <cotsb/server.h>
#include <thread>
#include <functional>
#include <sstream>
#include <unistd.h>
#include <cotsb/logging.h>

int main(int argc , char *argv[])
{
    cotsb::Server server(8888);
    server.start_server();

    cotsb::logger % "Info" << "Started server" << cotsb::endl;

    while (true)
    {
        server.check_network();

        for (const auto &iter : server.new_data())
        {
            server.broadcast(*iter.second.get());
        }
        server.clear_new_data();

        usleep(1000);

    }
    cotsb::logger % "Info" << "Done" << cotsb::endl;
}
 
