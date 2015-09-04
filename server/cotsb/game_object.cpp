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
        _angle(0.0f)
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
        auto new_pos = getPosition();
        new_pos.x += x;
        new_pos.y += y;

        if (!_current_map->can_move_to(this, new_pos))
        {
            return;
        }
        sf::Transformable::move(x, y);
        _location_moved = true;
    }
    void GameObject::move(const sf::Vector2f &dir)
    {
        auto new_pos = getPosition() + dir;

        if (!_current_map->can_move_to(this, new_pos))
        {
            return;
        }
        sf::Transformable::move(dir);
        _location_moved = true;
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
