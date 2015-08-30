#include "player.h"

#include "map.h"

#include "server_engine.h"
#include <cotsb/commands.h>

#include <exception>

namespace cotsb
{
    // Player {{{
    Player::Player(uint32_t id, sf::TcpSocket *socket) :
        _id(id),
        _socket(socket),
        _game_object(nullptr)
    {

    }

    uint32_t Player::id() const
    {
        return _id;
    }
    sf::TcpSocket *Player::socket() const
    {
        return _socket;
    }

    void Player::player_name(const std::string &name)
    {
        _player_name = name;
    }
    const std::string &Player::player_name() const
    {
        return _player_name;
    }

    void Player::colour(sf::Color colour)
    {
        _colour = colour;
    }
    sf::Color Player::colour() const
    {
        return _colour;
    }

    void Player::game_object(GameObject *obj)
    {
        _game_object = obj;
    }
    GameObject *Player::game_object() const
    {
        return _game_object;
    }

    bool Player::has_seen_game_obj(uint32_t id) const
    {
        return _seen_game_objs.find(id) != _seen_game_objs.cend();
    }
    void Player::add_seen_game_obj(uint32_t id)
    {
        _seen_game_objs.insert(id);
    }
    // }}}
    
    // PlayerManager {{{
    PlayerManager::PlayerMap PlayerManager::s_players; 
    PlayerManager::PlayerIdMap PlayerManager::s_player_ids;
    uint32_t PlayerManager::s_player_counter = 1u;

    const PlayerManager::PlayerMap &PlayerManager::players()
    {
        return s_players;
    }

    Player *PlayerManager::create_player(sf::TcpSocket *socket)
    {
        auto find = s_players.find(socket);
        if (find == s_players.end())
        {
            auto id = s_player_counter++;
            auto player = new Player(id, socket);
            s_players[socket] = std::unique_ptr<Player>(player);
            s_player_ids[id] = player;
            return player;
        }

        // Player already exists for socket.
        throw std::runtime_error("Player already exists for socket");
    }

    Player *PlayerManager::player(sf::TcpSocket *socket)
    {
        auto find = s_players.find(socket);
        if (find == s_players.end())
        {
            return nullptr;
        }
        return find->second.get();
    }
    Player *PlayerManager::player(uint32_t id)
    {
        auto find = s_player_ids.find(id);
        if (find == s_player_ids.end())
        {
            return nullptr;
        }
        return find->second;
    }

    void PlayerManager::remove_player(sf::TcpSocket *socket)
    {
        auto find = s_players.find(socket);
        if (find != s_players.end())
        {
            auto player = find->second.get();
            auto &goodbye = ServerEngine::broadcast(Commands::PlayerLeft);
            goodbye << player->player_name();

            auto map = player->game_object()->current_map();
            map->remove_player(player);

            GameObjectManager::remove_game_object(player->game_object());
            s_players.erase(find);
        }
    }
    // }}}
}
