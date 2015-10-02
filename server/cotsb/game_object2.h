#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

#include "component.h"

namespace cotsb
{
    class GameObject2
    {
        public:
            GameObject2(uint32_t id);

            inline uint32_t id() const
            {
                return _id;
            }

            typedef std::vector<std::unique_ptr<Component> > ComponentList;
            const ComponentList &components() const;

            void add_component(Component *comp);
            
            template <class T>
            void add_component()
            {
                auto id = Component::next_id();
                auto comp = new T(id);
                _components.push_back(std::unique_ptr<Component>(comp));
            }

            template <class T>
            inline T *component()
            {
                for (auto i = 0u; i < _components.size(); i++)
                {
                    if (_components[i]->type() == T::component_type)
                    {
                        return dynamic_cast<T *>(_components[i].get());
                    }
                }

                return nullptr;
            }

            template <class T>
            inline std::vector<T *> components()
            {
                std::vector<T *> result;

                for (auto i = 0u; i < _components.size(); i++)
                {
                    if (_components[i]->type() == T::component_type)
                    {
                        result.push_back(_components[i].get());
                    }
                }

                return result;
            }

            void remove_component(Component *comp)
            {
                for (auto i = 0u; i < _components.size(); i++)
                {
                    if (_components[i].get() == comp)
                    {
                        _components.erase(_components.begin() + i);
                        break;
                    }
                }
            }

        private:
            uint32_t _id;
            ComponentList _components;

    };
}
