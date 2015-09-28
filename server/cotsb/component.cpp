#include "component.h"

namespace cotsb
{
    // Component {{{
    Component::Component(uint32_t id, Type type) :
        _id(id),
        _type(type)
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
