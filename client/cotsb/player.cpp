#include "player.h"

#include "client_engine.h"

namespace cotsb
{
    // Player {{{
    Player::Player() :
        _id(0u),
        _loaded(false),
        _game_object(nullptr),
        _current_map(nullptr)
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
    
    void Player::current_map(Map *map)
    {
        _current_map = map;
    }
    Map *Player::current_map() const
    {
        return _current_map;
    }

    void Player::update(float dt)
    {
        _move_dir.x = _move_dir.y = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            _move_dir.x -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            _move_dir.x += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            _move_dir.y -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            _move_dir.y += 1;
        }

        if (_move_dir != _move_dir_server)
        {
            auto &packet = ClientEngine::client().send(Commands::MoveInDirection);
            packet << _move_dir.x << _move_dir.y;

            _move_dir_server = _move_dir;
        }
    }
    // }}}
}
