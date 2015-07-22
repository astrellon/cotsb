#pragma once

#include "client.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace cotsb
{
    class ClientEngine
    {
        public:
            ClientEngine(sf::RenderWindow *window);

            bool init();
            void game_loop();
            void deinit();

        private:

            sf::RenderWindow *_window;
            cotsb::Client _client;
    };
}
