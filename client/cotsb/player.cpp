#include "player.h"

namespace cotsb
{
    // Player {{{
    Player::Player() :
        GameObject(),
        _loaded(false),
    {

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
    // }}}
}
