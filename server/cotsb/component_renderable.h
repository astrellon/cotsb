#pragma once

#include "component.h"

namespace cotsb
{
    class ComponentRenderable : public Component
    {
        public:
            ComponentRenderable(uint32_t id);

            static const Component::Type component_type = Component::Render;
    };
}
