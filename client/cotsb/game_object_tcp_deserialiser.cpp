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

        input >> id;
        input >> map_name;

        logger % "Info" << "New game object: " << id << ", in map " << map_name << endl;

        auto game_obj = GameObjectManager::create_game_object<GameObject>(id);
        auto map = MapManager::map(map_name);
        map->add_game_object(game_obj);
        game_obj->current_map(map);
        utils::deserialise_transformable(*game_obj, input);

        return game_obj;
    }
}
