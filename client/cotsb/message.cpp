#include "message.h"

namespace cotsb
{
    // Message {{{
    Message::Message(const std::string &message, GameObject *game_object) :
        _message(message),
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
    // }}}
}
