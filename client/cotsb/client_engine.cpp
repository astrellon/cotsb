#include "client_engine.h"

#include "texture_manager.h"
#include "font_manager.h"
#include <utils/utils.h>

#include <iostream>

namespace cotsb
{
    ClientEngine::ClientEngine(sf::RenderWindow *window) :
        _window(window)
    {

    }

    // Init {{{
    bool ClientEngine::init()
    {
        if (!cotsb::FontManager::init())
        {
            std::cout << "Failed to load fonts on startup\n";
            return false;
        }

        if (!cotsb::TextureManager::init())
        {
            std::cout << "Failed to load textures on startup\n";
            return false;
        }

        _client.start_client();

        return true;
    }
    // }}}

    // Game loop {{{
    void ClientEngine::game_loop()
    {

        sf::Clock clock;
        while (_window->isOpen())
        {
            // Process events {{{
            sf::Event event;
            while (_window->pollEvent(event))
            {
                // Close window: exit
                if (event.type == sf::Event::Closed)
                {
                    _window->close();
                }
            }
            // }}}

            // Game loop {{{
            sf::Time elapsed = clock.restart();
            auto dt = elapsed.asSeconds();

            _client.game_tick();
            if (_client.new_data().getDataSize() > 0)
            {
                std::cout << "Has " << _client.new_data().getDataSize() << " bytes\n";
                std::string message;
                _client.new_data() >> message;
                std::cout << "Message: " << message << "\n";
            }

            utils::Utils::update(dt);
            // }}}

            // Draw game {{{
            // Clear screen
            _window->clear();

            sf::RenderStates states;

            // Update the window
            _window->display();
            // }}}
        }

    }
    // }}}

    // Deinit {{{
    void ClientEngine::deinit()
    {
        cotsb::FontManager::deinit();
        cotsb::TextureManager::deinit();
    }
    // }}}
}
