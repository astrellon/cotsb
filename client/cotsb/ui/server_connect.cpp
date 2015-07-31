#include "server_connect.h"

#include "button.h"
#include "text_input.h"
#include "ui_state.h"

#include <cotsb/client_engine.h>

namespace cotsb
{
    namespace ui
    {
        ServerConnect server_connect;

        bool ServerConnect::init()
        {
            _back = add_component<Button>("Back");
            _back->setPosition(300, 200);
            _back->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                State::state(State::MainMenu);
                
                return true;
            });

            _connect = add_component<Button>("Connect");
            _connect->setPosition(300, 260);
            _connect->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                return true;
            });

            _hostname = add_component<TextInput>("Host");
            _hostname->setPosition(300, 320);

            _port = add_component<TextInput>("Port");
            _port->setPosition(300, 380);

            calc_nearby_components();

            return true;
        }
    }
}
