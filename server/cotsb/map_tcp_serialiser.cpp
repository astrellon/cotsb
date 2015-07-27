#include "map_tcp_serialiser.h"

namespace cotsb
{
    void MapTcpSerialiser::serialise(const Map &map, sf::Packet &output)
    {
        output << "map" << map.width() << map.height();
        for (const auto &tile : map.data())
        {
            output << tile;
        }
    }
}
