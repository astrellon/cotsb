#include "map.h"

#include "client_engine.h"
#include <cotsb/commands.h>

namespace cotsb
{
    // Map {{{
    Map::Map(const std::string &name, uint32_t width, uint32_t height) :
        _name(name),
        _width(width),
        _height(height)
    {
        auto total = width * height;
        _data.resize(total);
        for (auto i = 0u; i < total; i++ )
        {
            _data[i] = "unknown";
        }
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
            auto map_status = status(name);
            if (map_status == NotLoading)
            {
                s_statuses[name] = Loading;
                auto &data = ClientEngine::client().send(Commands::LOAD_MAP);
                data << name;
            }
            return nullptr;
        }
        return find->second.get();
    }

    MapManager::Status MapManager::status(const std::string &name)
    {
        auto find = s_statuses.find(name);
        if (find == s_statuses.end())
        {
            return NotLoading;
        }
        return find->second;
    }
    const MapManager::Statuses &MapManager::statuses()
    {
        return s_statuses;
    }
    // }}}
}
