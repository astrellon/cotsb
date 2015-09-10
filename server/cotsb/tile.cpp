#include "tile.h"

#include <cotsb/logging.h>

namespace cotsb
{
    // Tile {{{
    Tile::Tile(const std::string &name, uint16_t id) :
        _name(name),
        _id(id),
        _passable(true),
        _colour(sf::Color(255, 0, 255, 255))
    {

    }

    void Tile::init(bool passable, const sf::Color &colour)
    {
        _passable = passable;
        _colour = colour;
    }

    std::string Tile::name() const
    {
        return _name;
    }
    uint16_t Tile::id() const
    {
        return _id;
    }

    bool Tile::passable() const
    {
        return _passable;
    }
    sf::Color Tile::colour() const
    {
        return _colour;
    }
    // }}}

    // TileManager {{{
    uint16_t TileManager::s_id_counter = 0u;
    TileManager::TileNameMap TileManager::s_name_map;
    TileManager::TileIdMap TileManager::s_id_map;
    Tile *TileManager::NullTile = nullptr;

    void TileManager::init()
    {
        NullTile = create("null", 0u);
        NullTile->init(false, sf::Color::Black);

        create("grass")->init(true, sf::Color::Green);
        create("wall")->init(false, sf::Color::Red);
        create("water")->init(false, sf::Color::Blue);
        create("dirt")->init(true, sf::Color::Yellow);
    }

    Tile *TileManager::tile(const std::string &name)
    {
        auto find = s_name_map.find(name);
        if (find != s_name_map.end())
        {
            return find->second.get();
        }
        return nullptr;
    }
    Tile *TileManager::tile(uint16_t id)
    {
        auto find = s_id_map.find(id);
        if (find != s_id_map.end())
        {
            return find->second;
        }
        return nullptr;
    }

    Tile *TileManager::create(const std::string &name)
    {
        auto find = s_name_map.find(name);
        if (find != s_name_map.end())
        {
            logger % "Error" << "A tile with the same name exists: " << name << endl;
            return nullptr;
        }

        auto id = ++s_id_counter;
        return create(name, id);
    }
    Tile *TileManager::create(const std::string &name, uint16_t id)
    {
        auto new_tile = new Tile(name, id);
        s_name_map[name] = std::unique_ptr<Tile>(new_tile);
        s_id_map[id] = new_tile;
        return new_tile;
    }
    // }}}
}
