#include <SFML/System.hpp>

#include <cotsb/server.h>
#include <cotsb/logging.h>

int main(int argc , char *argv[])
{
    cotsb::LoggerManager::init();

    cotsb::Server server(8888);
    server.start_server();

    cotsb::logger % "Info" << "Started server" << cotsb::endl;

    sf::Time sleep_time = sf::milliseconds(1);
    while (true)
    {
        server.check_network();

        for (const auto &iter : server.new_data())
        {
            server.broadcast(*iter.second.get());
        }
        server.clear_new_data();

        sf::sleep(sleep_time);

    }
    cotsb::logger % "Info" << "Done" << cotsb::endl;
}
 
