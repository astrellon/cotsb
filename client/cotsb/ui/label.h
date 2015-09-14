#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "component.h"

namespace cotsb
{
    namespace ui
    {
        class Label : public Component
        {
            public:
                Label(const std::string &text);

                void text(const std::string &text);
                std::string text() const;

                void colour(const sf::Color &colour);
                sf::Color colour() const;

                void width(float value);
                float width() const;

                void height(float value);
                float height() const;

                virtual void update(float dt);
                virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
                virtual sf::Vector2f size() const;

            private:
                std::string _text;
                sf::Text _text_graphic;

                float _width;
                float _height;

                void update_label_position();
        };
    }
}
