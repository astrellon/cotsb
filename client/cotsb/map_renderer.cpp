#include "map_renderer.h"

#include "map.h"
#include "game_object.h"
#include "tile.h"

namespace cotsb
{
    MapRenderer::MapRenderer() :
        _map(nullptr),
        _verticies(sf::Triangles)
    {

    }

    void MapRenderer::map(Map *map)
    {
        _map = map;
        create_vertex_array();
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

        target.draw(_verticies, states);

        const auto game_objects = _map->game_objects();
        for (auto obj : game_objects)
        {
            target.draw(*obj, states);
        }
    }

    void MapRenderer::create_vertex_array()
    {
        _verticies.clear();
        if (_map == nullptr)
        {
            return;
        }

        auto total = _map->width() * _map->height() * 6u;
        _verticies.resize(total);

        auto i = 0u;
        for (auto y = 0u; y < _map->height(); y++)
        {
            for (auto x = 0u; x < _map->width(); x++)
            {
                const auto tile = _map->tile(x, y);
                const auto c = tile->colour();
                set_vertex(_verticies[i++], x, y, 0, 0, c);
                set_vertex(_verticies[i++], x, y, 1, 0, c);
                set_vertex(_verticies[i++], x, y, 0, 1, c);
                set_vertex(_verticies[i++], x, y, 0, 1, c);
                set_vertex(_verticies[i++], x, y, 1, 0, c);
                set_vertex(_verticies[i++], x, y, 1, 1, c);
            }
        }
    }

    void MapRenderer::set_vertex(sf::Vertex &vert, float x, float y, 
            float offsetX, float offsetY, sf::Color colour)
    {
        vert.position = sf::Vector2f(x + offsetX, y + offsetY);
        vert.texCoords = sf::Vector2f(offsetX, offsetY);
        vert.color = colour;
    }
}
