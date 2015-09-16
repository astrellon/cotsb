#include "profile_lua_serialiser.h"

#include <utils/lua_serialiser.h>
#include <cotsb/logging.h>

#include "profile.h"

namespace cotsb
{
    Profile *ProfileLuaSerialiser::deserialise(const std::string &filename)
    {
        utils::UData profile_lua(utils::LuaSerialiser::deserialise(filename));
        if (profile_lua.get() != nullptr)
        {
            return deserialise(*profile_lua);
        }
        logger % "Error" << "Failed to load lua" << endl;
        return nullptr;
    }

    Profile *ProfileLuaSerialiser::deserialise(const utils::Data &input)
    {
        auto profile_name = input.at("profile_name")->string();
        auto display_name = input.at("display_name")->string();
        auto map_name = input.at("map_name")->string();
        auto loc_tuple = input.at("location");
        sf::Vector2f location(loc_tuple->at(1)->number(), loc_tuple->at(2)->number());

        auto profile = ProfileManager::create_profile(profile_name);
        profile->display_name(display_name);
        profile->map_name(map_name);
        profile->location(location);

        return profile;
    }

    utils::Data *ProfileLuaSerialiser::serialise(Profile *profile)
    {
        auto result = new utils::Data(utils::Data::Map);
        
        result->at("profile_name", profile->profile_name());
        result->at("display_name", profile->display_name());
        result->at("map_name", profile->map_name());

        auto loc = new utils::Data(utils::Data::Array);
        loc->push(profile->location().x);
        loc->push(profile->location().y);
        result->at("location", loc);

        return result;
    }
}
