#pragma once

#include <SFML/Network.hpp>

#include "game_object.h"

namespace cotsb
{
    class GameObjectTcpDeserialiser
    {
        public:
            static GameObject *deserialise(sf::Packet &input);
    };
}
