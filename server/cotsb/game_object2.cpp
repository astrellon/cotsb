#include "game_object2.h"

namespace cotsb
{
    GameObject2::GameObject2(uint32_t id) :
        _id(id)
    {

    }

    const GameObject2::ComponentList &GameObject2::components() const
    {
        return _components;
    }

    void GameObject2::add_component(Component *comp)
    {
        _components.push_back(std::unique_ptr<Component>(comp));
    }
}
