#include "player_tcp_deserialiser.h"

#include <cotsb/logging.h>
#include <utils/serialisers.h>

namespace cotsb
{
    bool PlayerTcpDeserialiser::deserialise(Player &player, sf::Packet &input)
    {
        std::string player_name;
        uint32_t colour_uint;
        uint32_t game_object_id;

        input >> player_name >> colour_uint >> game_object_id;
        player.player_name(player_name);

        auto colour = utils::uint_to_colour(colour_uint);

        logger % "Info" << "Player info: " << player_name << ", " << 
            colour_uint << ", " << game_object_id << endl;

        auto game_object = GameObjectManager::game_object(game_object_id);
        game_object->colour(colour);
        player.game_object(game_object);

        return true;
    }
}
