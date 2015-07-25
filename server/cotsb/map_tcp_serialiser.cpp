#include "map_tcp_serialiser.h"

namespace cotsb
{
    void MapTcpSerialiser::serialise(const Map &map, sf::Packet &output)
    {
        output << map.name() << map.width() << map.height();
    }
}
