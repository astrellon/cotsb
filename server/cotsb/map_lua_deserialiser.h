#pragma once

#include <string>
#include <utils/data.h>

namespace cotsb
{
    class Map;

    class MapLuaDeserialiser
    {
        public:
            static Map *deserialise(const std::string &filename);
            static Map *deserialise(const utils::Data &input);
    };
}
