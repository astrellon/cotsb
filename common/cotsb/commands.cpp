#include "commands.h"

#include <sstream>

namespace cotsb
{
    Commands::TypeMap Commands::s_names = {
        { Commands::Unknown, "Unknown" },
        { Commands::Message, "Message" },
        
        { Commands::ChangeMap, "ChangeMap" },
        { Commands::NewMap, "NewMap" },
        { Commands::NewPlayer, "NewPlayer" },
        { Commands::PlayerLeft, "PlayerLeft" },
        { Commands::JoinedGame, "JoinedGame" },
        
        { Commands::LoadMap, "LoadMap" },
        { Commands::JoinGame, "JoinGame" },
    };

    std::string Commands::get_name(Commands::Type type)
    {
        const auto find = s_names.find(type);
        if (find == s_names.cend())
        {
            std::stringstream ss;
            ss << static_cast<uint16_t>(type);
            return ss.str();
        }
        return find->second;
    }
    
    Logger &operator << (Logger &lhs, Commands::Type type)
    {
        lhs << Commands::get_name(type);
        return lhs;
    }
}
