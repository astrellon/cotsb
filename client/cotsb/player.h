#pragma once

#include <string>

#include "map.h"

namespace cotsb
{
    // Player {{{
    class Player
    {
        public:
            Player();

            void player_name(const std::string &name);
            const std::string &player_name() const;

            void current_map(Map *map);
            Map *current_map() const;

            void loaded(bool value);
            bool loaded() const;

        private:
            bool _loaded;
            std::string _player_name;
            Map *_current_map;
    };
    // }}}
}
