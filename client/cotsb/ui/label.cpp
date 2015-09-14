#include "label.h"

#include <cotsb/font_manager.h>

#include <utils/utils.h>

namespace cotsb
{
    namespace ui
    {
        Label::Label(const std::string &text) :
            Component()
        {
            auto font = FontManager::font("sans");
            _text_graphic.setFont(*font);
            _text_graphic.setCharacterSize(16);
            _text_graphic.setColor(sf::Color::White);
            this->text(text);
        }
        
        void Label::text(const std::string &text)
        {
            _text = text;
            _text_graphic.setString(_text);

            update_label_position();
        }
        std::string Label::text() const
        {
            return _text;
        }

        void Label::colour(const sf::Color &colour)
        {
            _text_graphic.setColor(colour);
        }
        sf::Color Label::colour() const
        {
            return _text_graphic.getColor();
        }

        void Label::width(float value)
        {
            _width = value;
        }
        float Label::width() const
        {
            return _width;
        }

        void Label::height(float value)
        {
            _height = value;
        }
        float Label::height() const
        {
            return _height;
        }

        void Label::update(float dt)
        {

        }
        void Label::draw(sf::RenderTarget &target, sf::RenderStates states) const
        {
            states.transform *= getTransform();

            target.draw(_text_graphic, states);
        }
        sf::Vector2f Label::size() const
        {
            auto rect = _text_graphic.getLocalBounds(); 
            return sf::Vector2f(rect.width, rect.height);
        }
        
        void Label::update_label_position()
        {
            sf::Vector2u texture_size;
            auto text_size = _text_graphic.getLocalBounds();

            auto left = (_width - text_size.width) * 0.5f;
            auto top = (_height - text_size.height) * 0.5f - text_size.top;

            _text_graphic.setPosition(utils::Utils::round(left), utils::Utils::round(top));
        }
    }
}

