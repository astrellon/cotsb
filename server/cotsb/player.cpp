#include "player.h"

#include <exception>

namespace cotsb
{
    // Player {{{
    Player::Player() :
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
    // }}}
    
    // PlayerManager {{{
    PlayerManager::PlayerMap PlayerManager::s_players; 

    const PlayerManager::PlayerMap &PlayerManager::players()
    {
        return s_players;
    }

    Player *PlayerManager::create_player(const sf::TcpSocket *socket)
    {
        auto find = s_players.find(socket);
        if (find == s_players.end())
        {
            auto player = new Player();
            s_players[socket] = std::unique_ptr<Player>(player);
            return player;
        }

        // Player already exists for socket.
        throw std::runtime_error("Player already exists for socket");
    }

    Player *PlayerManager::player(const sf::TcpSocket *socket)
    {
        auto find = s_players.find(socket);
        if (find == s_players.end())
        {
            return nullptr;
        }
        return find->second.get();
    }
    // }}}
}
