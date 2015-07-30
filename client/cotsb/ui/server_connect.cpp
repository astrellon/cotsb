#include "server_connect.h"

#include <iostream>

#include "button.h"
#include "manager.h"
#include "text_input.h"
#include "ui_state.h"

#include <cotsb/texture_manager.h>
#include <cotsb/client_engine.h>

namespace cotsb
{
    namespace ui
    {
        MenuHelper ServerConnect::s_helper;
        TextInput *ServerConnect::s_hostname = nullptr;
        TextInput *ServerConnect::s_port = nullptr;
        Button *ServerConnect::s_connect = nullptr;

        bool ServerConnect::init()
        {
            auto back = new Button("Back");
            back->setPosition(300, 200);
            s_helper.back_button(back);
            back->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                State::state(State::MainMenu);
                
                return true;
            });

            auto connect_button = new Button("Connect");
            connect_button->setPosition(300, 260);
            s_helper.add_component(connect_button);
            connect_button->on_click([] (int32_t x, int32_t y, sf::Mouse::Button btn)
            {
                return true;
            });

            s_hostname = new TextInput("Hostname");
            s_hostname->setPosition(300, 320);
            s_helper.add_component(s_hostname);

            s_port = new TextInput("Port");
            s_port->setPosition(300, 380);
            s_helper.add_component(s_port);

            s_helper.calc_nearby_components();
            s_helper.on_resize();

            visible(false);

            return true;
        }

        void ServerConnect::visible(bool value)
        {
            s_helper.visible(value);
        }
        bool ServerConnect::visible()
        {
            return s_helper.visible();
        }

        void ServerConnect::on_resize( uint32_t width, uint32_t height )
        {
            s_helper.on_resize(width, height);
        }
    }
}
