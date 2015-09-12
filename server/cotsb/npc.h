#pragma once

#include <string>
#include <stdint.h>

namespace cotsb
{
    class GameObject;
    class Map;

    class NPC
    {
        public:
            NPC(uint16_t id);

            void name(const std::string &name);
            std::string name() const;

            void game_object(GameObject *obj);
            GameObject *game_object() const;

            void current_map(Map *map);
            Map *current_map() const;

        private:
            uint16_t _id;
            GameObject *_game_object;
            Map *_current_map;
    };

    class NPCManager
    {
        public:
    };
}
