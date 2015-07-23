#include "ui_state.h"

#include "main_menu.h"

namespace cotsb
{
    namespace ui
    {
        State::StateValue State::s_state = State::MAIN_MENU;

        bool State::init()
        {
            state(MAIN_MENU);

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
            if (prev_state == MAIN_MENU || prev_state == PAUSE_GAME)
            {
                MainMenu::visible(false);
            }

            if (new_state == MAIN_MENU || new_state == PAUSE_GAME)
            {
                MainMenu::visible(true);
            }
        }
    }
}

