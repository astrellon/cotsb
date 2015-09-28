#pragma once

#include "component.h"

#include <SFML/Graphics.hpp>

namespace cotsb
{
    class ComponentTransform : public Component, public sf::Transformable
    {
        public:
            ComponentTransform(uint32_t id);

            static const Component::Type component_type;
        private:
    };
}
