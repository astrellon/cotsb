#pragma once

#include "screen.h"

namespace cotsb
{
    namespace ui
    {
        class Button;

        class MainMenu : public Screen
        {
            public:
                virtual bool init();

            private:

                Button *_start_game;
                Button *_options;
                Button *_quit;
        };

        extern MainMenu main_menu;
    }
}
