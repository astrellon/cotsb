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

        auto profile = ProfileManager::create_profile(profile_name);
        profile->display_name(display_name);
        profile->map_name(map_name);
        
        auto loc_data = input.at("location");
        if (!loc_data->is_nil())
        {
            sf::Vector2f location(loc_data->at(0)->number(), loc_data->at(1)->number());
            profile->location(location);
        }
        
        auto colour_data = input.at("colour");
        if (!colour_data->is_nil())
        {
            sf::Color colour(colour_data->at(0)->int32(),
                    colour_data->at(1)->int32(),
                    colour_data->at(2)->int32(),
                    colour_data->at(3)->int32());
            profile->colour(colour);
        }

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

        auto colour = new utils::Data(utils::Data::Array);
        colour->push(profile->colour().r);
        colour->push(profile->colour().g);
        colour->push(profile->colour().b);
        colour->push(profile->colour().a);
        result->at("colour", colour);

        return result;
    }
}
