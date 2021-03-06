#pragma once

#include <string>

#include "game_object.h"

namespace cotsb
{
    class Map;

    // Player {{{
    class Player
    {
        public:
            Player();

            void id(uint16_t value);
            uint16_t id() const;

            void player_name(const std::string &name);
            const std::string &player_name() const;

            void loaded(bool value);
            bool loaded() const;

            void game_object(GameObject *value);
            GameObject *game_object() const;
            
            void current_map(Map *map);
            Map *current_map() const;

            void update(float dt);

        private:
            uint16_t _id;
            bool _loaded;
            std::string _player_name;
            GameObject *_game_object;
            Map *_current_map;

            sf::Vector2<int8_t> _move_dir;
            sf::Vector2<int8_t> _move_dir_server;
    };
    // }}}
}
