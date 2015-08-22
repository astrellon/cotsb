#include "game_object_tcp_serialiser.h"

#include <utils/serialisers.h>

namespace cotsb
{
    void GameObjectTcpSerialiser::serialise(const GameObject *obj, sf::Packet &output)
    {
        if (obj == nullptr)
        {
            output << 0u;
            return;
        }

        output << obj->id();

        utils::serialise_transformable(*obj, output);
    }
}
