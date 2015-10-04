#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>

#include "component.h"

namespace cotsb
{
    class ComponentRenderable : public Component
    {
        public:
            ComponentRenderable(uint32_t id);

            void colour(const sf::Color &colour);
            sf::Color colour() const;

            static const Component::Type component_type = Component::Render;

        private:
            sf::Color _colour;
    };
}
