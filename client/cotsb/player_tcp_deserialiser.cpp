#include "player_tcp_deserialiser.h"

#include <cotsb/logging.h>

namespace cotsb
{
    bool PlayerTcpDeserialiser::deserialise(Player &player, sf::Packet &input)
    {
        std::string player_name;
        std::string map_name;
        sf::Vector2f position;
        uint32_t colour_uint;

        input >> player_name >> map_name >> position.x >> position.y >> colour_uint;
        player.player_name(player_name);

        logger % "Info" << "Player info: " << player_name << ", " << 
            map_name << ", " << position.x << ", " << position.y << ", " << 
            colour_uint << endl;

        player.current_map(MapManager::map(map_name));

        return true;
    }
}
