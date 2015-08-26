#include "game_object.h"

#include "map.h"

namespace cotsb
{
    // GameObject {{{
    GameObject::GameObject(uint32_t id) :
        _id(id),
        _colour(255u, 0, 0),
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

    void GameObject::colour(const sf::Color &colour)
    {
        _colour = colour;
    }
    sf::Color GameObject::colour() const
    {
        return _colour;
    }

    void GameObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(1, 1));
        shape.setFillColor(_colour);
        shape.setOutlineColor(sf::Color::Black);
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

    void GameObjectManager::remove_game_object(GameObject *obj)
    {
        auto find = s_game_objects.find(obj->id());
        if (find != s_game_objects.end())
        {
            if (obj->current_map() != nullptr)
            {
                obj->current_map()->remove_game_object(obj);
            }
            s_game_objects.erase(find);
        }
    }
    // }}}
}
