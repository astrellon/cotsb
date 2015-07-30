#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include "object_manager.h"

namespace cotsb
{
    // Map {{{
    class Map
    {
        public:
            Map(const std::string &name, uint32_t width, uint32_t height);

            std::string name() const;
            uint32_t width() const;
            uint32_t height() const;

            void tile(uint32_t x, uint32_t y, const std::string &tile);
            std::string tile(uint32_t x, uint32_t y) const;

            typedef std::vector<std::string> TileList;
            const TileList &data() const;

        private:
            std::string _name;
            uint32_t _width;
            uint32_t _height;

            TileList _data;
    };
    // }}}

    // MapManager {{{
    class MapManager
    {
        public:
            static void init();

            typedef std::map<std::string, std::unique_ptr<Map> > Maps;
            static const Maps &maps();
            static Map *map(const std::string &name);

            enum Status
            {
                NotLoading,
                Loading,
                Error,
                Loaded
            };

            typedef std::map<std::string, Status> Statuses;
            static Status status(const std::string &name);
            static const Statuses &statuses();

        private:
            static Maps s_maps;
            static Statuses s_statuses;
    };
    // }}}
}