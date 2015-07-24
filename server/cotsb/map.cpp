#include "map.h"

namespace cotsb
{
    Map::Map(uint32_t width, uint32_t height) :
        _width(width),
        _height(height)
    {

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
}
