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
        MainMenu main_menu;

        bool MainMenu::init()
        {
            _start_game = add_component<Button>("Start Game");
            _start_game->setPosition(300, 200);
            _start_game->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                State::state(State::ServerConnect);
                
                return true;
            });

            _options = add_component<Button>("Options");
            _options->setPosition(300, 260);

            _quit = add_component<Button>("Quit");
            _quit->setPosition(300, 380);
            _quit->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                ClientEngine::close_game();
                
                return true;
            });

            calc_nearby_components();

            return true;
        }
    }
}
