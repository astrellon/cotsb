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
                _verticies[i    ].position = sf::Vector2f(x, y);
                _verticies[i    ].color = tile->colour();
                _verticies[i + 1].position = sf::Vector2f(x + 1, y);
                _verticies[i + 1].color = tile->colour();
                _verticies[i + 2].position = sf::Vector2f(x, y + 1);
                _verticies[i + 2].color = tile->colour();
                
                _verticies[i + 3].position = sf::Vector2f(x, y + 1);
                _verticies[i + 3].color = tile->colour();
                _verticies[i + 4].position = sf::Vector2f(x + 1, y);
                _verticies[i + 4].color = tile->colour();
                _verticies[i + 5].position = sf::Vector2f(x + 1, y + 1);
                _verticies[i + 5].color = tile->colour();

                i += 6u;
            }
        }
    }
}
