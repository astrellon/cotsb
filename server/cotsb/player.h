#pragma once

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <memory>

#include "game_object.h"

namespace cotsb
{
    class Map;

    // Player {{{
    class Player
    {
        public:
            Player(uint32_t id);

            uint32_t id() const;

            void player_name(const std::string &name);
            const std::string &player_name() const;

            void colour(sf::Color colour);
            sf::Color colour() const;

            void game_object(GameObject *obj);
            GameObject *game_object() const;

        private:
            uint32_t _id;
            std::string _player_name;
            sf::Color _colour;
            GameObject *_game_object;
    };
    // }}}

    // PlayerManager {{{
    class PlayerManager
    {
        public:
            typedef std::map<const sf::TcpSocket *, std::unique_ptr<Player> > PlayerMap;
            typedef std::map<uint32_t, Player *> PlayerIdMap;

            static const PlayerMap &players();
            static const PlayerIdMap &player_ids();

            static Player *create_player(const sf::TcpSocket *socket);
            static Player *player(const sf::TcpSocket *socket);
            static Player *player(uint32_t id);

        private:
            static PlayerMap s_players;
            static PlayerIdMap s_player_ids;
            static uint32_t s_player_counter;

    };
    // }}}
}
