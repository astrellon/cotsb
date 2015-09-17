#include "profile.h"

#include <exception>

#include "profile_lua_serialiser.h"
#include <utils/lua_serialiser.h>

#include "player.h"
#include "map.h"

namespace cotsb
{
    // Profile {{{
    Profile::Profile(const std::string &name) :
        _profile_name(name),
        _player(nullptr)
    {

    }

    std::string Profile::profile_name() const
    {
        return _profile_name;
    }

    void Profile::display_name(const std::string &name)
    {
        _display_name = name;
    }
    std::string Profile::display_name() const
    {
        return _display_name;
    }
    
    void Profile::map_name(const std::string &name)
    {
        _map_name = name;
    }
    std::string Profile::map_name() const
    {
        return _map_name;
    }
    
    void Profile::location(const sf::Vector2f &loc)
    {
        _location = loc;
    }
    sf::Vector2f Profile::location() const
    {
        return _location;
    }

    void Profile::player(Player *player)
    {
        _player = player;
    }
    Player *Profile::player() const
    {
        return _player;
    }

    void Profile::update_from_player()
    {
        if (_player == nullptr)
        {
            return;
        }
        
        if (_player->current_map() == nullptr)
        {
            _map_name = "null";
        }
        else
        {
            _map_name = _player->current_map()->name();
        }

        _location = _player->game_object()->getPosition();
    }
    // }}}
    
    // ProfileManager {{{
    ProfileManager::ProfileMap ProfileManager::s_profiles;

    void ProfileManager::init()
    {

    }
    void ProfileManager::deinit()
    {
        for (auto &profile : s_profiles)
        {
            save_profile(profile.second.get());
        }
    }

    const ProfileManager::ProfileMap &ProfileManager::profiles()
    {
        return s_profiles;
    }

    Profile *ProfileManager::profile(const std::string &name)
    {
        auto find = s_profiles.find(name);
        if (find == s_profiles.end())
        {
            return nullptr;
        }

        return find->second.get();
    }
    Profile *ProfileManager::find_load_profile(const std::string &name)
    {
        auto loaded = profile(name);
        if (loaded != nullptr)
        {
            return loaded;
        }

        std::string path("profiles/");
        path += name;
        path += ".lua";
        return ProfileLuaSerialiser::deserialise(path);
    }
    Profile *ProfileManager::create_profile(const std::string &name)
    {
        auto find = s_profiles.find(name);
        if (find != s_profiles.end())
        {
            throw std::runtime_error("A profile with that name already exists");
        }

        auto new_profile = new Profile(name);
        new_profile->display_name(name);
        s_profiles[name] = std::unique_ptr<Profile>(new_profile);
        return new_profile;
    }

    void ProfileManager::save_profile(Profile *profile)
    {
        std::string path("profiles/");
        path += profile->profile_name();
        path += ".lua";

        profile->update_from_player();
        utils::UData serialised(ProfileLuaSerialiser::serialise(profile));
        utils::LuaSerialiser::serialise(serialised.get(), path);
    }
    // }}}
}
