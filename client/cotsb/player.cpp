#include "player.h"

namespace cotsb
{
    // Player {{{
    Player::Player() :
        _id(0u),
        _loaded(false),
        _game_object(nullptr)
    {

    }

    void Player::id(uint32_t value)
    {
        _id = value;
    }
    uint32_t Player::id() const
    {
        return _id;
    }

    void Player::player_name(const std::string &name)
    {
        _player_name = name;
    }
    const std::string &Player::player_name() const
    {
        return _player_name;
    }

    void Player::loaded(bool value)
    {
        _loaded = value;
    }
    bool Player::loaded() const
    {
        return _loaded;
    }

    void Player::game_object(GameObject *value)
    {
        _game_object = value;
    }
    GameObject *Player::game_object() const
    {
        return _game_object;
    }
    // }}}
}
