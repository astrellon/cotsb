#pragma once

#include <string>

namespace cotsb
{
    class Map;

    class Player
    {
        public:
            Player();

            void player_name(const std::string &name);
            const std::string &player_name() const;

            void current_map(Map *map);
            Map *current_map() const;

        private:
            std::string _player_name;
            Map *_current_map;
    };
}
