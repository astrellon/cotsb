#include "component.h"

namespace cotsb
{
    // Component {{{
    uint32_t Component::s_id = 0u;

    Component::Component(uint32_t id, Type type) :
        _id(id),
        _type(type)
    {

    }
    Component::~Component()
    {

    }

    uint32_t Component::id() const
    {
        return _id;
    }
    Component::Type Component::type() const
    {
        return _type;
    }
    // }}}
}
