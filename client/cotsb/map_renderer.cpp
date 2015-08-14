#include "map_renderer.h"

#include "map.h"

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

        auto size = 32.0f;
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
                line.setSize(sf::Vector2f(size, size));
                line.setPosition(x * size, y * size);
                target.draw(line, states);
            }
        }
    }
}
