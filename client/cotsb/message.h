#pragma once

#include <SFML/Network.hpp>

#include <map>
#include <vector>
#include <stdint.h>
#include <memory>
#include <string>

namespace cotsb
{
    class GameObject;

    // Message {{{
    class Message
    {
        public:
            enum Type : uint8_t
            {
                Server = 1,
                ServerWhisper = 2,
                Say = 3,
                Whisper = 4
            };
            Message(const std::string &message, Type type, GameObject *game_object);

            const std::string &message() const;
            Type type() const;
            GameObject *game_object() const;

        private:
            std::string _message;
            Type _type;
            GameObject *_game_object;
    };
    // }}}

    // MessageManager {{{
    class MessageManager
    {
        public:
            typedef std::vector<std::unique_ptr<Message> > ManagedMessageList;
            typedef std::vector<Message> MessageList;

            static const ManagedMessageList &messages();
            static const MessageList &new_messages();
            static void clear_new_messages();

            static void process_packet(sf::Packet &packet);

        private:
            static ManagedMessageList s_messages;
            static MessageList s_new_messages;
    };
    // }}}
}
