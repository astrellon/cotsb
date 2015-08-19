#pragma once

#include <string>

#include "game_object.h"

namespace cotsb
{
    // Player {{{
    class Player : public GameObject
    {
        public:
            Player();

            void player_name(const std::string &name);
            const std::string &player_name() const;

            void loaded(bool value);
            bool loaded() const;

        private:
            bool _loaded;
            std::string _player_name;
    };
    // }}}
}
