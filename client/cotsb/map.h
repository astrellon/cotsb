#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <functional>

#include "object_manager.h"

namespace cotsb
{
    class GameObject;
    class Tile;

    // Map {{{
    class Map
    {
        public:
            Map(const std::string &name);

            std::string name() const;

            void set_size(uint32_t width, uint32_t height);
            uint32_t width() const;
            uint32_t height() const;

            void tile(uint32_t x, uint32_t y, Tile *tile);
            Tile *tile(uint32_t x, uint32_t y) const;
            
            enum Status
            {
                MapNotFound,
                NotLoading,
                Loading,
                Error,
                Loaded
            };
            void status(Status value);
            Status status() const;
            
            typedef std::vector<Tile *> TileList;
            const TileList &data() const;
            TileList &data();

            typedef std::vector<GameObject *> GameObjectList;
            const GameObjectList &game_objects() const;

            void add_game_object(GameObject *obj);
            void remove_game_object(GameObject *obj);

            void update(float dt);

        private:
            std::string _name;
            uint32_t _width;
            uint32_t _height;
            Status _status;

            TileList _data;
            GameObjectList _game_objects;
    };
    // }}}
    
    // MapManager {{{
    class MapManager
    {
        public:
            static void init();

            typedef std::map<std::string, std::unique_ptr<Map> > Maps;
            static const Maps &maps();
            static Map *map(const std::string &name, bool auto_request = true);
            static Map::Status status(const std::string &name);

            typedef std::function<void (Map *loadedMap)> MapLoadHandler;
            static void map_loaded(Map *map);
            static void on_map_load(const std::string &name, MapLoadHandler handler);

        private:
            static Maps s_maps;

            typedef std::map<std::string, MapLoadHandler> MapLoadHandlers;
            static MapLoadHandlers s_map_load_handlers;
    };
    // }}}
}
