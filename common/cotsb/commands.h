#pragma once

#include <stdint.h>

namespace cotsb
{
    class Commands
    {
        public:
            enum Type : uint16_t
            {
                UNKNOWN,
                NEW_MAP
            };
    };
}
