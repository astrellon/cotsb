#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <map>
#include <memory>

namespace cotsb
{
    class Map;

    // GameObject {{{
    class GameObject : public sf::Transformable
    {
        public:
            GameObject(uint32_t id);

            uint32_t id() const;
            
            void current_map(Map *map);
            Map *current_map() const;

            void move(float x, float y);
            void move(const sf::Vector2f &dir);
            void setPosition(float x, float y);
            void setPosition(const sf::Vector2f &location);

            bool location_moved() const;
            void clear_location_moved();

            virtual void update(float dt);

        private:
            uint32_t _id;
            bool _location_moved;
            Map *_current_map;
    };
    // }}}
    
    // GameObjectManager {{{
    class GameObjectManager
    {
        public:
            static GameObject *game_object(uint32_t id);

            typedef std::map<uint32_t, std::unique_ptr<GameObject> > GameObjectMap;
            static const GameObjectMap &game_objects();

            template <class T>
            static T *create_game_object()
            {
                auto id = s_game_object_counter++;
                auto new_obj = new T(id);
                s_game_objects[id] = std::unique_ptr<GameObject>(new_obj);
                return new_obj;
            }
            static void remove_game_object(GameObject *obj);

            static void check_for_updates();

        private:
            static GameObjectMap s_game_objects;

            static uint32_t s_game_object_counter;

    };
    // }}}
}
