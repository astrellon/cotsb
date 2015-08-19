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

        private:
            uint32_t _id;
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

        private:
            static GameObjectMap s_game_objects;

    };
    // }}}

}
