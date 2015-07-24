#pragma once

#include <SFML/Network.hpp>

#include "map.h"

namespace cotsb
{
    class MapTcpSerialiser
    {
        public:
            static void serialise(const Map &map, sf::Packet &output);
    };
}
