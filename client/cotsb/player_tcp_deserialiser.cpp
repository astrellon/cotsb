#include "player_tcp_deserialiser.h"

namespace cotsb
{
    bool PlayerTcpDeserialiser::deserialise(Player &player, sf::Packet &input)
    {
        std::string player_name;
        std::string map_name;

        input >> player_name >> map_name;
        player.player_name(player_name);

        player.current_map(MapManager::map(map_name));

        return true;
    }
}
