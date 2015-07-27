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
                MESSAGE,
                NEW_MAP,
                LOAD_MAP
            };
    };
}
