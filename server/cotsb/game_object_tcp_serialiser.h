#pragma once

#include <SFML/Network.hpp>
#include "game_object.h"

namespace cotsb
{
    class GameObjectTcpSerialiser
    {
        public:
            static void serialise(const GameObject *obj, sf::Packet &output);
    };
}
