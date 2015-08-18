#pragma once

#include <SFML/Network.hpp>

#include "player.h"

namespace cotsb
{
    class PlayerTcpDeserialiser
    {
        public:
            static bool deserialise(Player &player, sf::Packet &input);
    };
}
