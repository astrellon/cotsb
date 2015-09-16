#pragma once

#include <string>
#include <utils/data.h>
#include <iostream>

namespace cotsb
{
    class Profile;

    class ProfileLuaSerialiser
    {
        public:
            static Profile *deserialise(const std::string &filename);
            static Profile *deserialise(const utils::Data &data);

            static utils::Data *serialise(Profile *profile);
    };
}
