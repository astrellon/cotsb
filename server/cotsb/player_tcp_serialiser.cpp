#include "player_tcp_serialiser.h"

#include "map.h"
#include <utils/colour.h>

namespace cotsb
{
    void PlayerTcpSerialiser::serialise(const Player &player, sf::Packet &output)
    {
        output << player.player_name();
        auto map = player.current_map();
        if (map == nullptr)
        {
            output << "";
        }
        else
        {
            output << map->name();
        }

        output << player.location().x << player.location().y;
        output << utils::colour_to_uint(player.colour());
    }
}
