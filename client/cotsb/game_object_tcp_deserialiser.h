#pragma once

#include <SFML/Network.hpp>

#include "game_object.h"

namespace cotsb
{
    class GameObjectTcpDeserialiser
    {
        public:
            static bool deserialise(GameObject &obj, sf::Packet &input);
            static GameObject *deserialise(sf::Packet &input);
    };
}
