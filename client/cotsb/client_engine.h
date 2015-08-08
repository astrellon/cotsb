#pragma once

#include "client.h"
#include "game_world.h"
#include "sound.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace cotsb
{
    class ClientEngine
    {
        public:

            static bool init(sf::RenderWindow *window);
            static void deinit();
            
            static void game_loop();

            static inline sf::View &hud_camera()
            {
                return s_hud_camera;
            }
            static inline sf::Vector2i mouse_position()
            {
                return sf::Mouse::getPosition(*s_window);
            }
            static inline sf::Vector2u window_size()
            {
                return s_window->getSize();
            }

            static inline sf::RenderWindow *window()
            {
                return s_window;
            }

            static void close_game();

            static void process_event(const sf::Event &event);
            static void update(float dt);
            static void draw(sf::RenderTarget &target, sf::RenderStates states);

            static bool is_key_pressed(sf::Keyboard::Key key);
            static bool is_key_released(sf::Keyboard::Key key);
            
            static SoundManager &sound_manager();
            static Client &client();
            static GameWorld *game_world();
            
            static void start_client();
            static void start_client(const std::string &hostname, uint16_t port);

        private:

            static sf::RenderWindow *s_window;
            static Client s_client;
            static GameWorld *s_game_world;
            static sf::View s_hud_camera;
            static SoundManager s_sound_manager;
            
            static uint32_t s_update_counter;
            static uint32_t s_keys_pressed[sf::Keyboard::KeyCount];
            static uint32_t s_keys_released[sf::Keyboard::KeyCount];
            
            static void on_resize(uint32_t width, uint32_t height);
            static void key_pressed(sf::Keyboard::Key key);
            static void key_released(sf::Keyboard::Key key);

            static void process_networking();

            static void on_connected();
    };
}
