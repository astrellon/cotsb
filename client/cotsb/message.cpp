#include "message.h"

#include <cotsb/logging.h>

namespace cotsb
{
    // Message {{{
    Message::Message(const std::string &message, Type type, GameObject *game_object) :
        _message(message),
        _type(type),
        _game_object(game_object)
    {

    }

    const std::string &Message::message() const
    {
        return _message;
    }
    GameObject *Message::game_object() const
    {
        return _game_object;
    }
    // }}}
    
    // MessageManager {{{
    MessageManager::ManagedMessageList MessageManager::s_messages;
    MessageManager::MessageList MessageManager::s_new_messages;

    const MessageManager::ManagedMessageList &MessageManager::messages()
    {
        return s_messages;
    }
    const MessageManager::MessageList &MessageManager::new_messages()
    {
        return s_new_messages;
    }

    void MessageManager::clear_new_messages()
    {
        s_new_messages.clear();
    }

    void MessageManager::process_packet(sf::Packet &packet)
    {
        uint8_t type;
        std::string message;
        packet >> type;

        auto message_type = static_cast<Message::Type>(type);
        if (message_type == Message::Say)
        {
            uint32_t game_obj_id;
            packet >> game_obj_id >> message;

            logger % "Say" << game_obj_id << ": " << message << endl; 
        }
        else if (message_type == Message::Whisper)
        {
            uint32_t game_obj_id;
            packet >> game_obj_id >> message;

            logger % "Whisper" << game_obj_id << ": " << message << endl; 
        }
        else if (message_type == Message::ServerWhisper)
        {
            packet >> message;
            logger % "Server" << message << endl;
        }
        else if (message_type == Message::Server)
        {
            packet >> message;
            logger % "Server" << message << endl; 
        }
    }
    // }}}
}
