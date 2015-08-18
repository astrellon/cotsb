#pragma once

#include <SFML/Network.hpp>

#include "map.h"

namespace cotsb
{
    class MapTcpDeserialiser
    {
        public:
            static bool deserialise(Map &map, sf::Packet &input);
    };
}
