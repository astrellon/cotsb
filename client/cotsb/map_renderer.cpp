#include "map_renderer.h"

#include "map.h"
#include "game_object.h"

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

        // Do draw
        for (auto y = 0u; y < _map->height(); y++)
        {
            for (auto x = 0u; x < _map->width(); x++)
            {
                sf::RectangleShape line;

                const auto &tile = _map->tile(x, y);
                if (tile == "grass")
                {
                    line.setFillColor(sf::Color::Green);
                }
                else if (tile == "water")
                {
                    line.setFillColor(sf::Color::Blue);
                }
                else if (tile == "wall")
                {
                    line.setFillColor(sf::Color::Red);
                }
                else
                {
                    line.setFillColor(sf::Color::Magenta);
                }
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
