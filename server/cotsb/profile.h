#pragma once

#include <SFML/System.hpp>

#include <stdint.h>
#include <string>
#include <map>
#include <memory>

namespace cotsb
{
    // Profile {{{
    class Profile
    {
        public:
            Profile(const std::string &profile_name);

            std::string profile_name() const;

            void display_name(const std::string &name);
            std::string display_name() const;

            void map_name(const std::string &name);
            std::string map_name() const;
            
            void location(const sf::Vector2f &loc);
            sf::Vector2f location() const;

        private:
            std::string _profile_name;
            std::string _display_name;
            std::string _map_name;
            sf::Vector2f _location;
    };
    // }}}

    // ProfileManager {{{
    class ProfileManager
    {
        public:
            static void init();
            static void deinit();

            typedef std::map<std::string, std::unique_ptr<Profile> > ProfileMap;
            static const ProfileMap &profiles();

            static Profile *profile(const std::string &name);
            static Profile *find_load_profile(const std::string &name);
            static Profile *create_profile(const std::string &name);
            static void save_profile(Profile *profile);

        private:
            static ProfileMap s_profiles;

    };
    // }}}
}
