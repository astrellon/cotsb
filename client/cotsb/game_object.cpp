#include "game_object.h"

namespace cotsb
{
    // GameObject {{{
    GameObject::GameObject(uint32_t id) :
        _id(id),
        _current_map(nullptr)
    {

    }

    uint32_t GameObject::id() const
    {
        return _id;
    }
    
    void GameObject:: current_map(Map *map)
    {
        _current_map = map;
    }
    Map *GameObject::current_map() const
    {
        return _current_map;
    }

    void GameObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(32, 32));
        shape.setFillColor(sf::Color::Red);
        shape.setOutlineColor(sf::Color::Black);
        shape.setPosition(-16, -16);
        target.draw(shape, states);
    }
    // }}}

    // GameObjectManager {{{
    GameObjectManager::GameObjectMap GameObjectManager::s_game_objects;

    GameObject *GameObjectManager::game_object(uint32_t id)
    {
        auto find = s_game_objects.find(id);
        if (find == s_game_objects.end())
        {
            return nullptr;
        }
        return find->second.get();
    }

    const GameObjectManager::GameObjectMap &GameObjectManager::game_objects()
    {
        return s_game_objects;
    }
    // }}}
}