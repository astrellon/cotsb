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
        _state(Lobby),
        _socket(socket),
        _game_object(nullptr),
        _current_map(nullptr)
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

    void Player::state(Player::State state)
    {
        _state = state;
    }
    Player::State Player::state() const
    {
        return _state;
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

    void Player::current_map(Map *map)
    {
        _current_map = map;
    }
    Map *Player::current_map() const
    {
        return _current_map;
    }

    void Player::move(int8_t x, int8_t y)
    {
        _move_dir.x = x;
        _move_dir.y = y;
    }

    bool Player::has_seen_game_obj(uint32_t id) const
    {
        return _seen_game_objs.find(id) != _seen_game_objs.cend();
    }
    void Player::add_seen_game_obj(uint32_t id)
    {
        _seen_game_objs.insert(id);
    }

    void Player::update(float dt)
    {
        sf::Vector2f move(_move_dir);
        move *= dt;
        _game_object->move(move);
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

            auto map = player->current_map();
            map->remove_player(player);

            GameObjectManager::remove_game_object(player->game_object());
            s_players.erase(find);
        }
    }

    void PlayerManager::update(float dt)
    {
        for (auto &iter : s_players)
        {
            if (iter.second->state() != Player::Ready)
            {
                continue;
            }
            iter.second->update(dt);
        }
    }
    // }}}
}
