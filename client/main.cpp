#include <cotsb/client.h>
#include <cotsb/client_engine.h>
#include <cotsb/logging.h>

int main(int argc , char *argv[])
{
    // Create the main window
    cotsb::LoggerManager::init();

    cotsb::logger % "Info" << "Starting client" << cotsb::endl;
    sf::RenderWindow window(sf::VideoMode(800, 600), "cotsb", sf::Style::Default);

    if (!cotsb::ClientEngine::init(&window))
    {
        cotsb::logger % "Error" << "Failed to init client engine!" << cotsb::endl;
        return 0;
    }
    cotsb::ClientEngine::game_loop();

    cotsb::logger % "Info" << "Shutting game down" << cotsb::endl;
    cotsb::ClientEngine::deinit();

    return EXIT_SUCCESS;
}
