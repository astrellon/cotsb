#include "component_transform.h"

namespace cotsb
{
    Component::Type ComponentTransform::component_type = Component::Transform;

    ComponentTransform::ComponentTransform(uint32_t id) :
        Component(id, component_type)
    {

    }
}
