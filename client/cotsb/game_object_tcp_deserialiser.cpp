#include "game_object_tcp_deserialiser.h"

#include <cotsb/logging.h>
#include <utils/serialisers.h>
#include "map.h"

namespace cotsb
{
    GameObject *GameObjectTcpDeserialiser::deserialise( sf::Packet &input )
    {
        uint32_t id;
        std::string map_name;
        float size;
        uint32_t colour_uint;

        input >> id >> map_name >> colour_uint >> size;

        logger % "Info" << "New game object: " << id << ", in map " << map_name << 
            " with colour: " << colour_uint << endl;

        auto game_obj = GameObjectManager::create_game_object<GameObject>(id);
        game_obj->size(size);
        auto map = MapManager::map(map_name);
        map->add_game_object(game_obj);
        game_obj->current_map(map);

        auto colour = utils::uint_to_colour(colour_uint);
        game_obj->colour(colour);

        utils::deserialise_transformable(*game_obj, input);

        return game_obj;
    }
}
