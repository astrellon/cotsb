#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>

namespace cotsb
{
    class Map;

    class GameObject : public sf::Transformable
    {
        public:
            GameObject(uint32_t id);

            uint32_t id() const;
            
            void current_map(Map *map);
            Map *current_map() const;

        private:
            uint32_t _id;
    };
}
