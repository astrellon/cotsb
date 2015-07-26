#include "map.h"

namespace cotsb
{
    // Map {{{
    Map::Map(const std::string &name, uint32_t width, uint32_t height) :
        _name(name),
        _width(width),
        _height(height)
    {

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
    // }}}
    
    // MapManager {{{
    MapManager::Maps MapManager::s_maps;
    MapManager::Statuses MapManager::s_statuses;

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
            
        }
    }
    // }}}
}
