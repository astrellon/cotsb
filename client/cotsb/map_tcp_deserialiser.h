#pragma once

#include <SFML/Network.hpp>
#include <string>

#include "map.h"

namespace cotsb
{
    class MapTcpDeserialiser
    {
        public:
            static Map *deserialise(sf::Packet &input);
    };
}
