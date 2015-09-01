#include "player_tcp_serialiser.h"

#include "map.h"
#include <utils/serialisers.h>

#include "game_object_tcp_serialiser.h"

namespace cotsb
{
    void PlayerTcpSerialiser::serialise(const Player &player, sf::Packet &output)
    {
        output << player.player_name();
        output << utils::colour_to_uint(player.colour());
        output << player.current_map()->name();
        output << player.game_object()->id();
    }
}
