#include "client_engine.h"

#include "texture_manager.h"
#include "font_manager.h"

#include "ui/manager.h"
#include "ui/ui_state.h"

#include <utils/utils.h>
#include <cotsb/logging.h>

#include "map.h"
#include "map_tcp_deserialiser.h"
#include <cotsb/commands.h>

namespace cotsb
{
    sf::RenderWindow *ClientEngine::s_window = nullptr;
    Client ClientEngine::s_client;
    sf::View ClientEngine::s_hud_camera;
    GameWorld *ClientEngine::s_game_world = nullptr;
    std::string ClientEngine::s_player_name;
    
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
            logger % "Error" << "Failed to load fonts on startup" << endl;
            return false;
        }

        if (!cotsb::TextureManager::init())
        {
            logger % "Error" << "Failed to load textures on startup" << endl;
            return false;
        }

        if (!ui::Manager::init())
        {
            logger % "Error" << "Failed to initialise UI" << endl;
            return -1;
        }

        if (!ui::State::init())
        {
            logger % "Error" << "Failed to initialise main UI" << endl;
            return -1;
        }
        
        if (!s_sound_manager.init())
        {
            logger % "Error" << "Failed to initialise sound engine." << endl;
            return -1;
        }

        //s_client.start_client();

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
                ui::Manager::process_event(event);
            }
            // }}}

            // Game loop {{{
            sf::Time elapsed = clock.restart();
            auto dt = elapsed.asSeconds();

            s_client.game_tick();
            process_networking();

            update(dt);
            utils::Utils::update(dt);
            ui::Manager::update(dt);
            // }}}

            // Draw game {{{
            // Clear screen
            s_window->clear();

            sf::RenderStates states;
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
        if (s_game_world != nullptr)
        {
            s_game_world->update(dt);
        }
        s_sound_manager.update(dt);
    }

    void ClientEngine::draw(sf::RenderTarget &target, sf::RenderStates states)
    {
        if (s_game_world != nullptr)
        {
            s_game_world->draw(target, states);
        }
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

        if (s_game_world != nullptr)
        {
            s_game_world->on_resize(width, height);
        }
    }
    
    SoundManager &ClientEngine::sound_manager()
    {
        return s_sound_manager;
    }
    Client &ClientEngine::client()
    {
        return s_client;
    }

    void ClientEngine::start_client()
    {
        s_client.start_client();
    }
    void ClientEngine::start_client(const std::string &hostname, uint16_t port)
    {
        s_client.hostname(hostname);
        s_client.port(port);

        start_client();
    }

    void ClientEngine::player_name(const std::string &name)
    {
        s_player_name = name;
    }
    const std::string &ClientEngine::player_name()
    {
        return s_player_name;
    }

    void ClientEngine::process_networking()
    {
        for (auto &iter : s_client.new_data())
        {
            auto &response = *iter.get();
            logger % "Network" << "Has " << response.data().getDataSize() << " bytes for " 
                << Commands::get_name(response.command()) << " commands" << endl;

            if (response.command() == Commands::NewMap)
            {
                logger % "Info" << "New map" << endl;
                auto map = MapTcpDeserialiser::deserialise(response.data());
                MapManager::map_loaded(map);
            }
            else if (response.command() == Commands::Message)
            {
                std::string message;
                response.data() >> message;
                logger % "Info" << "Message " << message << endl; 
            }
            else if (response.command() == Commands::JoinedGame)
            {
                on_joined_game(response);
            }
            else if (response.command() == Commands::NewPlayer)
            {
                std::string player_name;
                response.data() >> player_name;

                logger % "Info" << "New player: " << player_name << endl;
            }
            else if (response.command() == Commands::PlayerLeft)
            {
                std::string player_name;
                response.data() >> player_name;

                logger % "Info" << "Player left: " << player_name << endl;
            }
            else
            {
                logger % "Error" << "Unknown command " << response.command() << endl;
                continue;
            }
        }

        if (s_client.has_connected())
        {
            s_client.clear_has_connected();
            on_connected();
        }
    }

    void ClientEngine::on_connected()
    {
        auto &request = s_client.send(Commands::JoinGame);
        request << s_player_name;
    }

    void ClientEngine::on_joined_game(Client::Response &response)
    {
        logger % "Info" << "Joined game!" << endl;
        auto &request = s_client.send(Commands::LoadMap);
        request << "map1";

        MapManager::on_map_load("map1", [](Map *map)
        {
            logger % "Info" << "Got map1" << endl;
            s_game_world = new GameWorld();
            s_game_world->current_map(map);

            ui::State::state(ui::State::InGame);
        });
    }
}
