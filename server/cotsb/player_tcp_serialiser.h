#pragma once

#include <SFML/Network.hpp>

#include "player.h"

namespace cotsb
{
    class PlayerTcpSerialiser
    {
        public:
            static void serialise(const Player &player, sf::Packet &output);
    };
}
