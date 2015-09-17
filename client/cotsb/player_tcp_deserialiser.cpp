#include "player_tcp_deserialiser.h"

#include "map.h"

#include <cotsb/logging.h>
#include <utils/serialisers.h>

namespace cotsb
{
    bool PlayerTcpDeserialiser::deserialise(Player &player, sf::Packet &input)
    {
        std::string player_name;
        std::string map_name;
        uint32_t game_object_id;

        input >> player_name >> map_name >> game_object_id;
        player.player_name(player_name);

        logger % "Info" << "Player info: " << player_name << ", " << game_object_id << endl;
        auto game_object = GameObjectManager::game_object(game_object_id);
        player.game_object(game_object);
        player.current_map(MapManager::map(map_name));

        return true;
    }
}
