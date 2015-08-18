#include "player.h"

namespace cotsb
{
    // Player {{{
    Player::Player() :
        _loaded(false),
        _current_map(nullptr)
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

    void Player:: current_map(Map *map)
    {
        _current_map = map;
    }
    Map *Player::current_map() const
    {
        return _current_map;
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
