#pragma once

#include <string>

namespace cotsb
{
    class Message
    {
        public:
            enum Type
            {
                Server = 1,
                Say = 2,
                Whisper = 3
            };

            Message(const std::string &message, Type type);

        private:
            std::string _message;
            Type _type;
    };
}
