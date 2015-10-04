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
                Unknown = 0x00,
                Message = 0x01,

                // Server -> client
                ChangeMap = 0x02,
                NewMap = 0x03,
                NewPlayer = 0x04,
                PlayerLeft = 0x05,
                JoinedGame = 0x06,
                NewGameObject = 0x07,
                UpdateGameObject = 0x08,
                RemoveGameObject = 0x09,
                MoveGameObject = 0x0A,
                ProfileNotFound = 0x0B,

                // Client -> server
                LoadMap = 0x0C,
                JoinGame = 0x0D,
                LoadedPlayerMap = 0x0E,
                MoveInDirection = 0x0F,
                CreateProfile = 0x10,
            };

            static std::string get_name(Type type);

        private:
            typedef std::map<Type, std::string> TypeMap;
            static TypeMap s_names;
    };

    Logger &operator << (Logger &lhs, Commands::Type type);
}
