#include "main_menu.h"

#include "button.h"
#include "text_input.h"
#include "manager.h"
#include "ui_state.h"

#include <cotsb/texture_manager.h>
#include <cotsb/font_manager.h>
#include <cotsb/client_engine.h>
#include <utils/utils.h>

namespace cotsb
{
    namespace ui
    {
        Button *MainMenu::s_start_game = nullptr;
        Button *MainMenu::s_options = nullptr;
        Button *MainMenu::s_quit = nullptr;
        MenuHelper MainMenu::s_helper;

        bool MainMenu::init()
        {
            s_start_game = new Button("Start Game");
            s_start_game->setPosition(300, 200);
            /*
            s_start_game->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                State::state(State::SHIP_HANGER);
                
                return true;
            });
            */
            s_helper.add_component(s_start_game);

            s_options = new Button("Options");
            s_options->setPosition(300, 260);
            s_helper.add_component(s_options);

            s_quit = new Button("Quit");
            s_quit->setPosition(300, 380);
            s_quit->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                ClientEngine::close_game();
                
                return true;
            });
            s_helper.add_component(s_quit);

            s_helper.calc_nearby_components();

            auto size = ClientEngine::window_size();
            on_resize(size.x, size.y);
            return true;
        }

        void MainMenu::visible(bool value)
        {
            s_helper.visible(value);
        }
        bool MainMenu::visible()
        {
            return s_helper.visible();
        }

        void MainMenu::on_resize( uint32_t width, uint32_t height )
        {
            auto size = s_start_game->texture()->getSize();
            auto x = utils::Utils::round((static_cast<float>(width) - size.x) * 0.5f);

            set_pos(s_start_game, x);
            set_pos(s_options, x);
            set_pos(s_quit, x);
        }

        void MainMenu::set_pos(sf::Transformable *obj, float x)
        {
            obj->setPosition(x, obj->getPosition().y);
        }
    }
}
