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
            Player();

            void player_name(const std::string &name);
            const std::string &player_name() const;

            void colour(sf::Color colour);
            sf::Color colour() const;

        private:
            std::string _player_name;
            sf::Color _colour;
    };
    // }}}

    // PlayerManager {{{
    class PlayerManager
    {
        public:
            typedef std::map<const sf::TcpSocket *, std::unique_ptr<Player> > PlayerMap;
            static const PlayerMap &players();

            static Player *create_player(const sf::TcpSocket *socket);
            static Player *player(const sf::TcpSocket *socket);

        private:
            static PlayerMap s_players;

    };
    // }}}
}
