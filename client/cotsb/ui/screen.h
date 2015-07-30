#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

namespace cotsb
{
    namespace ui
    {
        class Component;
        class Button;

        class Screen
        {
            public:
                Screen();
                
                void visible(bool value);
                bool visible() const;

                template <class T>
                T &add_component()
                {
                    auto new_comp = new T();
                    _components.push_back(std::unique_ptr<Component>(new_comp));
                    return *new_comp;
                }
                template <class T>
                T &add_component(const std::string &label)
                {
                    auto new_comp = new T(label);
                    _components.push_back(std::unique_ptr<Component>(new_comp));
                    return *new_comp;
                }

                void calc_nearby_components();
                
                typedef std::vector<std::unique_ptr<Component> > ComponentList;
                const ComponentList &components() const;

                void on_resize(uint32_t width, uint32_t height);
                void on_resize();

            private:
                bool _visible;
                ComponentList _components;
        };
    }
}
