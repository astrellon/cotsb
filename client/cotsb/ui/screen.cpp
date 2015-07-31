#include "screen.h"

#include "component.h"
#include "button.h"

#include <cotsb/client_engine.h>
#include <cotsb/logging.h>

#include <utils/utils.h>

namespace cotsb
{
    namespace ui
    {
        Screen::Screen() :
            _visible(false)
        {
        }

        void Screen::visible(bool value)
        {
            _visible = value;
        }
        bool Screen::visible() const
        {
            return _visible;
        }

        const Screen::ComponentList &Screen::components() const
        {
            return _components;
        }
        void Screen::calc_nearby_components()
        {
            for (auto i = 0u; i < _components.size(); i++)
            {
                auto comp = _components[i].get();

                if (i == 0u)
                {
                    comp->to_above(_components.back().get());
                }
                else
                {
                    comp->to_above(_components[i - 1].get());
                }
                
                if (i == _components.size() - 1)
                {
                    comp->to_below(_components.front().get());
                }
                if (i > 0)
                {
                    _components[i - 1].get()->to_below(comp);
                }
            }
        }
        
        void Screen::on_resize(uint32_t width, uint32_t height)
        {
            auto fwidth = static_cast<float>(width);
            for (auto i = 0u; i < _components.size(); i++)
            {
                auto comp = _components[i].get();
                auto size = comp->size();
                auto x = utils::Utils::round((fwidth - size.x) * 0.5f);

                comp->setPosition(x, comp->getPosition().y);
            }
        }
        void Screen::on_resize()
        {
            auto size = ClientEngine::window_size();
            on_resize(size.x, size.y);
        }

        void Screen::set_pos(sf::Transformable *obj, float x)
        {
            obj->setPosition(x, obj->getPosition().y);
        }
    }
}
