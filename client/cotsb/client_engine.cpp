#include "client_engine.h"

#include "texture_manager.h"
#include "font_manager.h"

#include "ui/manager.h"
#include "ui/ui_state.h"

#include <utils/utils.h>

#include <iostream>

namespace cotsb
{
    sf::RenderWindow *ClientEngine::s_window = nullptr;
    Client ClientEngine::s_client;
    sf::View ClientEngine::s_hud_camera;
    
    SoundManager ClientEngine::s_sound_manager;

    uint32_t ClientEngine::s_update_counter = 0;
    uint32_t ClientEngine::s_keys_pressed[sf::Keyboard::KeyCount];
    uint32_t ClientEngine::s_keys_released[sf::Keyboard::KeyCount];

    // Init {{{
    bool ClientEngine::init(sf::RenderWindow *window)
    {
        s_window = window;
        s_hud_camera = window->getDefaultView();
        
        auto size = static_cast<int>(sf::Keyboard::KeyCount);
        for (auto i = 0; i < size; i++)
        {
            s_keys_pressed[i] = 0u;
            s_keys_released[i] = 0u;
        }

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

        if (!ui::Manager::init())
        {
            std::cout << "Failed to initialise UI\n";
            return -1;
        }

        if (!ui::State::init())
        {
            std::cout << "Failed to initialise main UI\n";
            return -1;
        }
        
        if (!s_sound_manager.init())
        {
            std::cout << "Failed to initialise sound engine.\n";
            return -1;
        }

        s_client.start_client();

        return true;
    }
    // }}}

    // Game loop {{{
    void ClientEngine::game_loop()
    {
        sf::Clock clock;
        while (s_window->isOpen())
        {
            // Process events {{{
            sf::Event event;
            while (s_window->pollEvent(event))
            {
                process_event(event);
            }
            ui::Manager::process_event(event);
            // }}}

            // Game loop {{{
            sf::Time elapsed = clock.restart();
            auto dt = elapsed.asSeconds();

            s_client.game_tick();
            if (s_client.new_data().getDataSize() > 0)
            {
                std::cout << "Has " << s_client.new_data().getDataSize() << " bytes\n";
                std::string message;
                s_client.new_data() >> message;
                std::cout << "Message: " << message << "\n";
            }

            utils::Utils::update(dt);
            ui::Manager::update(dt);
            // }}}

            // Draw game {{{
            // Clear screen
            s_window->clear();

            sf::RenderStates states;
            //vot::GameSystem::draw(window, states);
            draw(*s_window, states);
            ui::Manager::draw(*s_window, states);

            // Update the window
            s_window->display();
            // }}}
        }

    }
    // }}}

    // Deinit {{{
    void ClientEngine::deinit()
    {
        s_sound_manager.clear();

        ui::Manager::deinit();
        FontManager::deinit();
        TextureManager::deinit();
    }
    // }}}
    
    void ClientEngine::process_event(const sf::Event &event)
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
        {
            s_window->close();
            return;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            key_pressed(event.key.code);
        }

        if (event.type == sf::Event::KeyReleased)
        {
            key_released(event.key.code);
        }

        if (event.type == sf::Event::Resized)
        {
            on_resize(event.size.width, event.size.height);
        }
    }

    void ClientEngine::update(float dt)
    {
        s_update_counter++;
        /*
        if (s_game.get() != nullptr)
        {
            s_game->update(dt);
        }
        */
        s_sound_manager.update(dt);
    }

    void ClientEngine::draw(sf::RenderTarget &target, sf::RenderStates states)
    {
        /*
        if (s_game.get() != nullptr)
        {
            s_game->draw(target, states);
        }
        */
    }

    void ClientEngine::close_game()
    {
        s_window->close();
    }

    void ClientEngine::key_pressed(sf::Keyboard::Key key)
    {
        s_keys_pressed[key] = s_update_counter + 1;
    }
    bool ClientEngine::is_key_pressed(sf::Keyboard::Key key)
    {
        return s_keys_pressed[key] == s_update_counter;
    }
    void ClientEngine::key_released(sf::Keyboard::Key key)
    {
        s_keys_released[key] = s_update_counter + 1;
    }
    bool ClientEngine::is_key_released(sf::Keyboard::Key key)
    {
        return s_keys_released[key] == s_update_counter;
    }

    void ClientEngine::on_resize(uint32_t width, uint32_t height)
    {
        s_window->setSize(sf::Vector2u(width, height));
        
        auto fwidth = static_cast<float>(width);
        auto fheight = static_cast<float>(height);
        s_hud_camera.setSize(fwidth, fheight);
        s_hud_camera.setCenter(fwidth * 0.5f, fheight * 0.5f);
    }
    
    SoundManager *ClientEngine::sound_manager()
    {
        return &s_sound_manager;
    }

}
