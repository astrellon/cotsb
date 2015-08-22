#include "game_object_tcp_deserialiser.h"

namespace cotsb
{
    bool GameObjectTcpDeserialiser::deserialise( cotsb::GameObject &obj, sf::Packet &input )
    {
        uint32_t id;
        input >> id;
    }
}
