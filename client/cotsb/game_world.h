#pragma once

#include <SFML/Graphics.hpp>

#include "map_renderer.h"

namespace cotsb
{
    class Map;

    class GameWorld
    {
        public:
            GameWorld();

            void current_map(Map *map);
            Map *current_map() const;

            void draw(sf::RenderTarget &target, sf::RenderStates states);
            void update(float dt);

            void on_resize(uint32_t width, uint32_t height);

            void world_scale(float value);
            float world_scale() const;

        private:
            sf::View _camera;
            float _world_scale;

            Map *_current_map;
            MapRenderer _map_renderer;
    };
}
