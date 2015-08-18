#include "game_world.h"

#include "map.h"
#include "client_engine.h"

namespace cotsb
{
    GameWorld::GameWorld() :
        _current_map(nullptr)
    {
        auto window_size = ClientEngine::window_size();
        _camera.setSize(static_cast<float>(window_size.x), static_cast<float>(window_size.y));

        logger % "Info" << "Camera size: " << window_size.x << ", " << window_size.y << endl;
    }

    void GameWorld::current_map(Map *map)
    {
        _current_map = map;
        _map_renderer.map(map);
    }
    Map *GameWorld::current_map() const
    {
        return _current_map;
    }

    void GameWorld::draw( sf::RenderTarget &target, sf::RenderStates states )
    {
        target.setView(_camera);

        _map_renderer.draw(target, states);
    }
    void GameWorld::update(float dt)
    {
        if (_current_map != nullptr)
        {
            _camera.setCenter(sf::Vector2f());
            _camera.setRotation(0.0f);
            _current_map->update(dt);
        }
    }

    void GameWorld::on_resize( uint32_t width, uint32_t height )
    {
        auto fwidth = static_cast<float>(width);
        auto fheight = static_cast<float>(height);
        _camera.setSize(fwidth, fheight);
        
        logger % "Info" << "Camera size: " << width << ", " << height << endl;
    }
}