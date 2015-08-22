#include "game_object.h"

#include "map.h"

namespace cotsb
{
    // GameObject {{{
    GameObject::GameObject(uint32_t id) :
        sf::Transformable(),
        _id(id),
        _current_map(nullptr)
    {

    }

    uint32_t GameObject::id() const
    {
        return _id;
    }

    void GameObject::current_map(Map *map)
    {
        _current_map = map;
    }
    Map *GameObject::current_map() const
    {
        return _current_map;
    }
    // }}}
    
    // GameObjectManager {{{
    GameObjectManager::GameObjectMap GameObjectManager::s_game_objects;
    uint32_t GameObjectManager::s_game_object_counter = 1u;

    GameObject *GameObjectManager::game_object(uint32_t id)
    {
        auto find = s_game_objects.find(id);
        if (find != s_game_objects.end())
        {
            return find->second.get();
        }
        return nullptr;
    }

    const GameObjectManager::GameObjectMap &GameObjectManager::game_objects()
    {
        return s_game_objects;
    }
    // }}}
}
