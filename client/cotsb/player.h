#pragma once

#include <string>

#include "game_object.h"

namespace cotsb
{
    // Player {{{
    class Player
    {
        public:
            Player();

            void id(uint32_t value);
            uint32_t id() const;

            void player_name(const std::string &name);
            const std::string &player_name() const;

            void loaded(bool value);
            bool loaded() const;

            void game_object(GameObject *value);
            GameObject *game_object() const;

        private:
            uint32_t _id;
            bool _loaded;
            std::string _player_name;
            GameObject *_game_object;
    };
    // }}}
}
