#include "server_engine.h"

#include <cotsb/logging.h>

namespace cotsb
{
    Server ServerEngine::s_server;
    bool ServerEngine::s_running = false;

    bool ServerEngine::init()
    {
        if (s_running)
        {
            return true;
        }

        s_running = true;
        s_server.port(8888);
        s_server.start_server();
    
        logger % "Info" << "Started server" <<endl;

        return true;
    }
    void ServerEngine::shutdown()
    {
        s_running = false;
    }
    bool ServerEngine::is_running()
    {
        return s_running;
    }

    void ServerEngine::server_loop()
    {
        sf::Time sleep_time = sf::milliseconds(1);
        while (s_running)
        {
            s_server.check_network();

            for (const auto &iter : s_server.new_data())
            {
                s_server.broadcast(*iter.second.get());
            }
            s_server.clear_new_data();

            sf::sleep(sleep_time);
        }
    }
}
