#pragma once

#include "screen.h"

#include <map>
#include <stdint.h>

namespace cotsb
{
    namespace ui
    {
        class Label;

        class GameHud : public Screen
        {
            public:
                virtual bool init();

            private:

                std::map<uint16_t, Label *> _player_messages;
        };

        extern GameHud game_hud;
    }
}
