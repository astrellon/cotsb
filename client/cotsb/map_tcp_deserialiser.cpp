#include "map_tcp_deserialiser.h"

#include <cotsb/logging.h>

namespace cotsb
{
    Map *MapTcpDeserialiser::deserialise(sf::Packet &input)
    {
        std::string name;
        uint32_t width, height;
        input >> name >> width >> height;

        logger % "Info" << "New map: " << name << ", " << width << ", " << height << endl;
        auto result = new Map(name, width, height);
        for (auto y = 0u; y < height; y++)
        {
            for (auto x = 0u; x < width; x++)
            {
                std::string tile;
                input >> tile;
                result->tile(x, y, tile);
            }
        }
        return result;
    }
}
