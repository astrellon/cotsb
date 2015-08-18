#include "map_tcp_deserialiser.h"

#include <cotsb/logging.h>

namespace cotsb
{
    bool MapTcpDeserialiser::deserialise(Map &map, sf::Packet &input)
    {
        bool success;
        input >> success;
        if (!success)
        {
            std::string error_message;
            input >> error_message;
            logger % "Error" << "Failed to load map (" << map.name() << "): " << error_message << endl;

            map.status(Map::Error);
            return false;
        }
        uint32_t width, height;
        input >> width >> height;

        logger % "Info" << "New map: " << width << ", " << height << endl;
        map.set_size(width, height);
        for (auto y = 0u; y < height; y++)
        {
            for (auto x = 0u; x < width; x++)
            {
                std::string tile;
                input >> tile;
                map.tile(x, y, tile);
            }
        }

        map.status(Map::Loaded);
        return true;
    }
}
