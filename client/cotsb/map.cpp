#include "map.h"

#include "client_engine.h"
#include <cotsb/commands.h>

#include <exception>

namespace cotsb
{
    // Map {{{
    Map::Map(const std::string &name) :
        _name(name),
        _width(0u),
        _height(0u),
        _status(NotLoading)
    {

    }

    std::string Map::name() const
    {
        return _name;
    }

    void Map::set_size(uint32_t width, uint32_t height)
    {
        if (width == 0u || height == 0u)
        {
            throw std::runtime_error("Cannot resize a map to zero width or height");
        }
        if (_width != 0u && _height != 0u)
        {
            throw std::runtime_error("Cannot resize a map");
        }

        _width = width;
        _height = height;
        
        auto total = width * height;
        _data.resize(total);
        for (auto i = 0u; i < total; i++ )
        {
            _data[i] = "unknown";
        }
    }
    uint32_t Map::width() const
    {
        return _width;
    }
    uint32_t Map::height() const
    {
        return _height;
    }

    void Map::status(Map::Status value)
    {
        _status = value;
    }
    Map::Status Map::status() const
    {
        return _status;
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

    void Map::update(float dt)
    {

    }
    // }}}
    
    // MapManager {{{
    MapManager::Maps MapManager::s_maps;
    MapManager::MapLoadHandlers MapManager::s_map_load_handlers;

    void MapManager::init()
    {

    }

    const MapManager::Maps &MapManager::maps()
    {
        return s_maps;
    }
    Map *MapManager::map(const std::string &name, bool auto_request)
    {
        auto find = s_maps.find(name);
        if (find == s_maps.end())
        {
            auto map = new Map(name);
            s_maps[name] = std::unique_ptr<Map>(map);

            if (auto_request)
            {
                map->status(Map::Loading);
                auto &request = ClientEngine::client().send(Commands::LoadMap);
                request << name;
            }

            return map;
        }
        return find->second.get();
    }

    Map::Status MapManager::status(const std::string &name)
    {
        auto find = s_maps.find(name);
        if (find == s_maps.cend())
        {
            return Map::MapNotFound;
        }
        return find->second->status();
    }

    void MapManager::map_loaded(Map *map)
    {
        auto find = s_map_load_handlers.find(map->name());
        if (find != s_map_load_handlers.end())
        {
            find->second(map);
            s_map_load_handlers.erase(find);
        }
    }

    void MapManager::on_map_load(const std::string &name, MapManager::MapLoadHandler handler)
    {
        if (status(name) == Map::Loaded)
        {
            handler(s_maps[name].get());
            return;
        }
        s_map_load_handlers[name] = handler;
    }
        
    // }}}
}
