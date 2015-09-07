#include "game_object.h"

#include "map.h"
#include "server_engine.h"

#include <cotsb/commands.h>

#include <cmath>

namespace cotsb
{
    // GameObject {{{
    GameObject::GameObject(uint32_t id) :
        sf::Transformable(),
        _id(id),
        _location_moved(false),
        _current_map(nullptr),
        _angle(0.0f),
        _size(0.75f)
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

    void GameObject::move(float x, float y)
    {
        move(sf::Vector2f(x, y));
    }
    void GameObject::move(const sf::Vector2f &dir)
    {
        auto pos = getPosition();
        auto new_pos = getPosition();

        auto left = pos.x;
        auto right = pos.x + _size;
        auto top = pos.y;
        auto bottom = pos.y + _size;

        auto xcheck = dir.x > 0 ? right : left;
        auto ycheck = dir.y > 0 ? bottom : top;

        if (dir.x != 0)
        {
            if (_current_map->can_move_to(this, sf::Vector2f(xcheck + dir.x, top)) &&
                _current_map->can_move_to(this, sf::Vector2f(xcheck + dir.x, bottom)))
            {
                new_pos.x += dir.x;
                _location_moved = true;
            }
        }
        if (dir.y != 0)
        {
            if (_current_map->can_move_to(this, sf::Vector2f(left, ycheck + dir.y)) &&
                _current_map->can_move_to(this, sf::Vector2f(right, ycheck + dir.y)))
            {
                new_pos.y += dir.y;
                _location_moved = true;
            }
        }

        sf::Transformable::setPosition(new_pos);
    }
    void GameObject::setPosition(float x, float y)
    {
        sf::Transformable::setPosition(x, y);
        _location_moved = true;
    }
    void GameObject::setPosition(const sf::Vector2f &location)
    {
        sf::Transformable::setPosition(location);
        _location_moved = true;
    }

    bool GameObject::location_moved() const
    {
        return _location_moved;
    }
    void GameObject::clear_location_moved()
    {
        _location_moved = false;
    }

    void GameObject::size(float value)
    {
        _size = value;
    }
    float GameObject::size() const
    {
        return _size;
    }

    void GameObject::update(float dt)
    {
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

    void GameObjectManager::remove_game_object(GameObject *obj)
    {
        auto find = s_game_objects.find(obj->id());
        if (find != s_game_objects.end())
        {
            if (obj->current_map() != nullptr)
            {
                obj->current_map()->remove_game_object(obj);
            }
            auto &packet = ServerEngine::broadcast(Commands::RemoveGameObject);
            packet << obj->id();

            s_game_objects.erase(find);
        }
    }

    void GameObjectManager::check_for_updates()
    {
        for (auto &pair : s_game_objects)
        {
            auto obj = pair.second.get();
            if (obj->location_moved())
            {
                auto map = obj->current_map();
                auto &move_packet = ServerEngine::send_callback(Commands::MoveGameObject,
                    [map] (Packet &packet)
                    {
                        for (auto &player : map->players())
                        {
                            player->socket()->send(*packet.data());
                        }
                    });

                auto &loc = obj->getPosition();
                move_packet << obj->id() << loc.x << loc.y;

                obj->clear_location_moved();
            }
        }
    }
    // }}}
}
