#pragma once

#include <SFML/Graphics.hpp>

namespace cotsb
{
    namespace utils
    {
        class Colour
        {
            public:
                Colour();
                Colour(const sf::Color &colour);

                operator sf::Color() const;

                void red(float value);
                float red() const;

                void green(float value);
                float green() const;

                void blue(float value);
                float blue() const;

                void rgb(float r, float g, float b);

                void hue(float value);
                float hue() const;

                void saturation(float value);
                float saturation() const;

                void value(float value);
                float value() const;

                void hsv(float h, float s, float v);

                void calc_rgb();
                void calc_hsv();

                void display();

            private:
                float _red;
                float _green;
                float _blue;
                float _hue;
                float _saturation;
                float _value;

                bool _dirty;
        };

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
    }
}
