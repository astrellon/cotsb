#pragma once

#include <stdint.h>

namespace cotsb
{
    // Component {{{
    class Component
    {
        public:

            enum Type : uint8_t
            {
                Transform,
                Render,
                Character,
                Inventory
            };

            Component(uint32_t id, Type type);
            Component(const Component &copy) = delete;
            virtual ~Component();

            uint32_t id() const;
            Type type() const;

            inline static uint32_t next_id()
            {
                return ++s_id;
            }

        private:
            uint32_t _id;
            Type _type;

            static uint32_t s_id;
    };
    // }}}
}
