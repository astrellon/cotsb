#include "ui_state.h"

#include "main_menu.h"
#include "server_connect.h"

namespace cotsb
{
    namespace ui
    {
        State::StateValue State::s_state = State::MainMenu;

        bool State::init()
        {
            state(MainMenu);

            return true; 
        }

        void State::state(State::StateValue state)
        {
            change_state(s_state, state);
            s_state = state;
        }
        State::StateValue State::state()
        {
            return s_state;
        }

        void State::change_state(StateValue prev_state, StateValue new_state)
        {
            if (prev_state == MainMenu || prev_state == PauseGame)
            {
                main_menu.visible(false);
            }
            if (prev_state == ServerConnect)
            {
                server_connect.visible(false);
            }

            if (new_state == MainMenu || new_state == PauseGame)
            {
                main_menu.visible(true);
            }
            if (new_state == ServerConnect)
            {
                server_connect.visible(true);
            }
        }
    }
}

