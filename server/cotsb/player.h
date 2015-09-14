#pragma once

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <memory>
#include <set>

#include "game_object.h"

namespace cotsb
{
    class Map;
    class Profile;

    // Player {{{
    class Player
    {
        public:

            enum State
            {
                Lobby,
                LoadingMap,
                Ready
            };

            Player(uint16_t id, sf::TcpSocket *socket);

            uint16_t id() const;
            sf::TcpSocket *socket() const;

            void state(State state);
            State state() const;

            void player_name(const std::string &name);
            const std::string &player_name() const;

            void colour(sf::Color colour);
            sf::Color colour() const;

            void game_object(GameObject *obj);
            GameObject *game_object() const;

            void current_map(Map *map);
            Map *current_map() const;

            void move(int8_t x, int8_t y);

            typedef std::set<uint32_t> GameObjectIdSet;
            bool has_seen_game_obj(uint32_t id) const;
            void add_seen_game_obj(uint32_t id);

            void update(float dt);

            void profile(Profile *profile);
            Profile *profile() const;

        private:
            uint16_t _id;
            State _state;
            sf::TcpSocket *_socket;
            std::string _player_name;
            sf::Color _colour;
            GameObject *_game_object;
            Map *_current_map;

            sf::Vector2<int8_t> _move_dir;

            GameObjectIdSet _seen_game_objs;
            Profile *_profile;
    };
    // }}}

    // PlayerManager {{{
    class PlayerManager
    {
        public:
            typedef std::map<sf::TcpSocket *, std::unique_ptr<Player> > PlayerMap;
            typedef std::map<uint16_t, Player *> PlayerIdMap;

            static const PlayerMap &players();
            static const PlayerIdMap &player_ids();

            static Player *create_player(sf::TcpSocket *socket);
            static Player *player(sf::TcpSocket *socket);
            static Player *player(uint16_t id);
            static void remove_player(sf::TcpSocket *socket);

            static void update(float dt);

        private:
            static PlayerMap s_players;
            static PlayerIdMap s_player_ids;
            static uint16_t s_player_counter;

    };
    // }}}
}
