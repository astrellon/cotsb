#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <map>
#include <memory>

#include <SFML/Graphics.hpp>

namespace cotsb
{
    class GameObject;
    class Player;
    class Tile;

    // Map {{{
    class Map
    {
        public:
            Map(const std::string &name, uint32_t width, uint32_t height);

            std::string name() const;
            uint32_t width() const;
            uint32_t height() const;

            void tile(uint32_t x, uint32_t y, Tile *tile);
            Tile *tile(uint32_t x, uint32_t y) const;

            typedef std::vector<Tile *> TileList;
            const TileList &data() const;

            typedef std::vector<GameObject *> GameObjectList;
            const GameObjectList &game_objects() const;

            typedef std::vector<Player *> PlayerList;
            const PlayerList &players() const;

            void add_game_object(GameObject *obj);
            void remove_game_object(GameObject *obj);
            bool can_move_to(GameObject *obj, const sf::Vector2f &position) const;

            void add_player(Player *player);
            void remove_player(Player *player);

            void update(float dt);

        private:
            std::string _name;
            uint32_t _width;
            uint32_t _height;

            TileList _data;
            GameObjectList _game_objects;
            PlayerList _players;
    };
    // }}}

    // MapManager {{{
    class MapManager
    {
        public:
            static void init();

            typedef std::map<std::string, std::unique_ptr<Map> > Maps;
            static const Maps &maps();
            static Map *map(const std::string &name);
            static void map(Map *map);

            static void update(float dt);

        private:
            static Maps s_maps;
    };
    // }}}
}
