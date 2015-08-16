#pragma once

#include <string>
#include <map>
#include <memory>

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

    class PlayerManager
    {
        public:
            typedef std::map<std::string, std::unique_ptr<Player> > PlayerMap;
            static const PlayerMap &players();

            static Player *player(const std::string &name);

        private:
            static PlayerMap s_players;

    };
}
