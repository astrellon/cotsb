#include "map.h"

#include "game_object.h"

namespace cotsb
{
    // Map {{{
    Map::Map(const std::string &name, uint32_t width, uint32_t height) :
        _name(name),
        _width(width),
        _height(height)
    {
        _data.resize(width * height);
    }

    std::string Map::name() const
    {
        return _name;
    }
    uint32_t Map::width() const
    {
        return _width;
    }
    uint32_t Map::height() const
    {
        return _height;
    }

    void Map::tile(uint32_t x, uint32_t y, const std::string &tile)
    {
        if (x < _width && y < _height)
        {
            _data[y * _width + x] = tile;
        }
    }
    std::string Map::tile(uint32_t x, uint32_t y) const
    {
        if (x >= _width || y >= _height)
        {
            return std::string();
        }

        return _data[y * _width + x];
    }

    const Map::TileList &Map::data() const
    {
        return _data;
    }

    const Map::GameObjectList &Map::game_objects() const
    {
        return _game_objects;
    }
    const Map::PlayerList &Map::players() const
    {
        return _players;
    }

    void Map::add_game_object(GameObject *obj)
    {
        _game_objects.push_back(obj);
    }
    void Map::remove_game_object(GameObject *obj)
    {
        for (auto iter = _game_objects.begin(); iter != _game_objects.end(); ++iter)
        {
            if (*iter == obj)
            {
                _game_objects.erase(iter);
                break;
            }
        }
    }

    void Map::add_player(Player *value)
    {
        _players.push_back(value);
    }
    void Map::remove_player(Player *value)
    {
        for (auto iter = _players.begin(); iter != _players.end(); ++iter)
        {
            if (*iter == value)
            {
                _players.erase(iter);
                break;
            }
        }
    }

    void Map::update(float dt)
    {
        for (auto &obj : _game_objects)
        {
            obj->update(dt);
        }
    }
    // }}}
    
    // MapManager {{{
    MapManager::Maps MapManager::s_maps;

    void MapManager::init()
    {

    }

    const MapManager::Maps &MapManager::maps()
    {
        return s_maps;
    }
    Map *MapManager::map(const std::string &name)
    {
        auto find = s_maps.find(name);
        if (find == s_maps.end())
        {
            return nullptr;
        }
        return find->second.get();
    }
    void MapManager::map(Map *map)
    {
        s_maps[map->name()] = std::unique_ptr<Map>(map);
    }

    void MapManager::update(float dt)
    {
        for (auto &map : s_maps)
        {
            map.second->update(dt);
        }
    }
    // }}}
}
