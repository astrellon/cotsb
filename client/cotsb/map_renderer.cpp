#include "map_renderer.h"

#include "map.h"
#include "game_object.h"
#include "tile.h"

namespace cotsb
{
    MapRenderer::MapRenderer() :
        _map(nullptr)
    {

    }

    void MapRenderer::map(Map *map)
    {
        _map = map;
    }
    Map *MapRenderer::map() const
    {
        return _map;
    }

    void MapRenderer::draw( sf::RenderTarget &target, sf::RenderStates states )
    {
        if (_map == nullptr)
        {
            return;
        }

        sf::RectangleShape line;
        // Do draw
        for (auto y = 0u; y < _map->height(); y++)
        {
            for (auto x = 0u; x < _map->width(); x++)
            {
                const auto tile = _map->tile(x, y);
                line.setFillColor(tile->colour());
                line.setSize(sf::Vector2f(1, 1));
                line.setPosition(x, y);
                target.draw(line, states);
            }
        }

        const auto game_objects = _map->game_objects();
        for (auto obj : game_objects)
        {
            target.draw(*obj, states);
        }
    }
}
