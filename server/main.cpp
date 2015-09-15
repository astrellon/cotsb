#include <SFML/Graphics.hpp>

#include <cotsb/server_engine.h>
#include <cotsb/logging.h>

int main(int argc , char *argv[])
{
    cotsb::LoggerManager::init();

    if (!cotsb::ServerEngine::init())
    {
        cotsb::logger % "Error" << "Unable to start server" << cotsb::endl;
        return 1;
    }
    cotsb::ServerEngine::server_loop();

    cotsb::logger % "Info" << "Shutting down" << cotsb::endl;
    cotsb::ServerEngine::deinit();

    return 0;
}
 
