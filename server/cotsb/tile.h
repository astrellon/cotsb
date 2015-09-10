#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <stdint.h>
#include <map>
#include <memory>

namespace cotsb
{
    // Tile {{{
    class Tile
    {
        public:
            Tile(const std::string &name, uint16_t id);

            void init(bool passable, const sf::Color &colour);

            std::string name() const;
            uint16_t id() const;
            bool passable() const;
            sf::Color colour() const;

        private:
            std::string _name;
            uint16_t _id;
            bool _passable;
            sf::Color _colour;
    };
    // }}}

    // TileManager {{{
    class TileManager
    {
        public:
            static void init();

            static Tile *tile(const std::string &name); 
            static Tile *tile(uint16_t id);

            static Tile *create(const std::string &name);

            typedef std::map<std::string, std::unique_ptr<Tile> > TileNameMap;
            typedef std::map<uint16_t, Tile *> TileIdMap;

            static Tile *NullTile;

        private:
            static uint16_t s_id_counter;

            static TileNameMap s_name_map;
            static TileIdMap s_id_map;

            static Tile *create(const std::string &name, uint16_t id);

    };
    // }}}
}
