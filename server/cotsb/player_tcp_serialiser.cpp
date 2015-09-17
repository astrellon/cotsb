#include "player_tcp_serialiser.h"

#include "map.h"
#include "profile.h"

#include "game_object_tcp_serialiser.h"

namespace cotsb
{
    void PlayerTcpSerialiser::serialise(const Player &player, sf::Packet &output)
    {
        output << player.profile()->display_name();
        output << player.current_map()->name();
        output << player.game_object()->id();
    }
}
