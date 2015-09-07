#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <map>
#include <memory>

namespace cotsb
{
    class Map;

    // GameObject {{{
    class GameObject : public sf::Drawable, public sf::Transformable
    {
        public:
            GameObject(uint32_t id);

            uint32_t id() const;
            
            void current_map(Map *map);
            Map *current_map() const;

            void colour(const sf::Color &colour);
            sf::Color colour() const;

            void size(float value);
            float size() const;

            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        private:
            uint32_t _id;
            sf::Color _colour;
            Map *_current_map;
            float _size;
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
            static T *create_game_object(uint32_t id)
            {
                auto new_obj = new T(id);
                s_game_objects[id] = std::unique_ptr<GameObject>(new_obj);
                return new_obj;
            }
            static void remove_game_object(GameObject *obj);
            static void remove_game_object(uint32_t id);

        private:
            static GameObjectMap s_game_objects;

    };
    // }}}

}
