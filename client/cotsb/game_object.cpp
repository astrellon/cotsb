#include "game_object.h"

namespace cotsb
{
    GameObject::GameObject() :
        _current_map(nullptr)
    {

    }
    
    void GameObject:: current_map(Map *map)
    {
        _current_map = map;
    }
    Map *GameObject::current_map() const
    {
        return _current_map;
    }

}
