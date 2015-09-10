#include "map_lua_deserialiser.h"

#include <utils/lua_serialiser.h>
#include <cotsb/logging.h>

#include "map.h"
#include "tile.h"

namespace cotsb
{
    Map *MapLuaDeserialiser::deserialise(const std::string &filename)
    {
        utils::UData map_lua(utils::LuaSerialiser::deserialise(filename));
        if (map_lua.get() != nullptr)
        {
            return deserialise(*map_lua.get());
        }
        logger % "Error" << "Failed to load lua" << endl;
        return nullptr;
    }

    Map *MapLuaDeserialiser::deserialise(utils::Data &input)
    {
        auto name = input.at("name")->string();
        auto tiles = input.at("tiles");
        auto height = tiles->size_array();

        if (height == 0u)
        {
            logger % "Error" << "Map has a tile height of 0" << endl;
            return nullptr;
        }

logger % "Info" << "Tiles: " << tiles->type_name() << endl;
        auto firstrow = tiles->at(0); 
        logger % "Info" << "Map first row: " << firstrow->type_name() << ", " << firstrow->is_nil() << endl;
        auto width = firstrow->size_array();

        auto result = new Map(name, width, height);

        for (auto y = 0u; y < height; y++)
        {
            auto tile_row = tiles->at(y);
            for (auto x = 0u; x < tile_row->size_array(); x++)
            {
                auto tile_name = tile_row->at(x)->string();
                auto tile = TileManager::tile(tile_name);
                result->tile(x, y, tile);
            }
        }

        return result;
    }
}
