#pragma once

#include <stdint.h>
#include <map>
#include <string>

#include <cotsb/logging.h>

namespace cotsb
{
    typedef uint8_t CommandType;
    class Commands
    {
        public:
            enum Type : CommandType
            {
                // Common
                Unknown,
                Message,

                // Server -> client
                ChangeMap,
                NewMap,
                NewPlayer,
                PlayerLeft,
                JoinedGame,
                NewGameObject,
                UpdateGameObject,
                RemoveGameObject,
                MoveGameObject,

                // Client -> server
                LoadMap,
                JoinGame,
            };

            static std::string get_name(Type type);

        private:
            typedef std::map<Type, std::string> TypeMap;
            static TypeMap s_names;
    };

    Logger &operator << (Logger &lhs, Commands::Type type);
}
