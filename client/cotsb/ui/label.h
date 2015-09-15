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

                virtual void update(float dt);
                virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
                
                void size(const sf::Vector2f &size);
                void size(float width, float height);
                virtual sf::Vector2f size() const;

            private:
                std::string _text;
                sf::Text _text_graphic;
                sf::Vector2f _size;

                void update_label_position();
        };
    }
}
