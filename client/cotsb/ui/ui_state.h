#pragma once

namespace cotsb
{
    namespace ui
    {
        class State
        {
            public:
                enum StateValue
                {
                    MainMenu,
                    InGame,
                    PauseGame,
                    ServerConnect
                };

                static bool init();

                static void state(StateValue state);
                static StateValue state();

            private:
                static StateValue s_state;
                static void change_state(StateValue prev_state, StateValue new_state);
        };
    }
}
