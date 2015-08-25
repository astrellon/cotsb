#include "game_object_tcp_serialiser.h"

#include <utils/serialisers.h>
#include "map.h"

namespace cotsb
{
    void GameObjectTcpSerialiser::serialise(const GameObject *obj, sf::Packet &output)
    {
        output << obj->id();
        output << obj->current_map()->name();

        utils::serialise_transformable(*obj, output);
    }
}
