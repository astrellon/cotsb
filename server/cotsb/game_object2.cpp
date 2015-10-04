#include "game_object2.h"

#include "map.h"

namespace cotsb
{
    GameObject2::GameObject2(uint32_t id) :
        _id(id),
        _map(nullptr)
    {

    }

    void GameObject2::map(Map *map)
    {
        _map = map;
    }
    Map *GameObject2::map() const
    {
        return _map;
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
