#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>

namespace cotsb
{
    class Map;

    class MapRenderer
    {
        public:
            MapRenderer();

            void map(Map *map);
            Map *map() const;

            void draw( sf::RenderTarget &target, sf::RenderStates states );

        private:
            Map *_map;
            sf::VertexArray _verticies;

            void create_vertex_array();
            void set_vertex(sf::Vertex &vert, float x, float y, 
                    float offsetX, float offsetY, sf::Color colour);
    };
}
