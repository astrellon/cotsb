#pragma once

#include <stdint.h>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

namespace cotsb
{
    namespace utils
    {
        inline uint32_t colour_to_uint(const sf::Color &colour)
        {
            return colour.a << 24 | colour.r << 16 | colour.g << 8 | colour.b;
        }
        inline sf::Color uint_to_colour(uint32_t colour)
        {
            auto a = (colour >> 24) & 0xFF;
            auto r = (colour >> 16) & 0xFF;
            auto g = (colour >> 8) & 0xFF;
            auto b = colour & 0xFF;

            return sf::Color(r, g, b, a);
        }

        inline void serialise_transformable(const sf::Transformable &trans, sf::Packet &output)
        {
            output << trans.getPosition().x << trans.getPosition().y << trans.getRotation() << trans.getScale().x << trans.getScale().y;
        }
        inline void deserialise_transformable(sf::Transformable &trans, sf::Packet &input)
        {
            float x, y;
            input >> x >> y;
            trans.setPosition(x, y);

            input >> x;
            trans.setRotation(x);

            input >> x >> y;
            trans.setScale(x, y);
        }
    }
}
