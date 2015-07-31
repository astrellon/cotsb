#include "manager.h"

#include <cotsb/client_engine.h>
#include <cotsb/logging.h>

#include "screen.h"
#include "main_menu.h"
#include "server_connect.h"

namespace cotsb
{
    namespace ui
    {
        Manager::ScreenList Manager::s_screens;
        Component *Manager::s_has_focus = nullptr;
        Component *Manager::s_last_had_focus = nullptr;

        bool Manager::init()
        {
            if (!main_menu.init())
            {
                return false;
            }
            if (!server_connect.init())
            {
                return false;
            }

            s_screens.push_back(&main_menu);
            s_screens.push_back(&server_connect);
            
            auto size = ClientEngine::window_size();
            for (auto screen : s_screens)
            {
                screen->on_resize(size.x, size.y);
            }

            return true;
        }
        void Manager::deinit()
        {
            s_has_focus = nullptr;
            s_last_had_focus = nullptr;
        }

        void Manager::focus(Component *component)
        {
            if (s_has_focus != nullptr)
            {
                s_has_focus->has_focus(false);
            }
            s_has_focus = component;
            if (s_has_focus != nullptr)
            {
                s_has_focus->has_focus(true);
            }
        }
        Component *Manager::focus()
        {
            return s_has_focus;
        }

        void Manager::update(float dt)
        {
            for (auto screen : s_screens)
            {
                if (!screen->visible())
                {
                    continue;
                }

                for (auto &comp : screen->components())
                {
                    comp->update(dt);
                }
            }
        }

        void Manager::draw(sf::RenderTarget &target, sf::RenderStates states)
        {
            target.setView(ClientEngine::hud_camera());
            for (auto screen : s_screens)
            {
                if (!screen->visible())
                {
                    continue;
                }

                for (auto &comp : screen->components())
                {
                    if (comp->enabled())
                    {
                        if (comp->local_view() != nullptr)
                        {
                            target.setView(*comp->local_view());
                        }
                        target.draw(*comp, states);
                        if (comp->local_view() != nullptr)
                        {
                            target.setView(ClientEngine::hud_camera());
                        }
                    }
                }
            }
        }

        void Manager::process_event(const sf::Event &event)
        {
            if (event.type == sf::Event::MouseMoved)
            {
                check_hover(event.mouseMove.x, event.mouseMove.y);
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                check_pressed(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (s_has_focus != nullptr && s_has_focus->enabled())
                {
                    s_has_focus->do_text(event.text.unicode);
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                auto back = event.key.code == sf::Keyboard::W ||
                    event.key.code == sf::Keyboard::Up;
                auto forward = event.key.code == sf::Keyboard::S ||
                    event.key.code == sf::Keyboard::Down ||
                    event.key.code == sf::Keyboard::Tab;
                auto left = event.key.code == sf::Keyboard::A ||
                    event.key.code == sf::Keyboard::Left;
                auto right = event.key.code == sf::Keyboard::D ||
                    event.key.code == sf::Keyboard::Right;

                if (s_has_focus != nullptr && s_has_focus->enabled())
                {
                    if (s_has_focus->do_keypress(event.key.code))
                    {
                        if (back && s_has_focus->to_above() != nullptr)
                        {
                            change_focus(s_has_focus->to_above());
                        }
                        else if (forward && s_has_focus->to_below() != nullptr)
                        {
                            change_focus(s_has_focus->to_below());
                        }
                        else if (left && s_has_focus->to_left())
                        {
                            change_focus(s_has_focus->to_left());
                        }
                        else if (right && s_has_focus->to_right())
                        {
                            change_focus(s_has_focus->to_right());
                        }
                        else if (event.key.code == sf::Keyboard::Space ||
                                event.key.code == sf::Keyboard::Return) 
                        {
                            check_pressed(0, 0, sf::Mouse::Left);
                        }
                    }
                }
                else
                {
                    if (back || forward || left || right)
                    {
                        if (s_last_had_focus == nullptr || !s_last_had_focus->enabled())
                        {
                            for (auto screen : s_screens)
                            {
                                for (auto &comp : screen->components())
                                {
                                    if (comp->enabled())
                                    {
                                        change_focus(comp.get());
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            change_focus(s_last_had_focus);
                        }
                    }
                }
            }
            else if (event.type == sf::Event::Resized)
            {
                for (auto screen : s_screens) 
                {
                    screen->on_resize(event.size.width, event.size.height);
                }
            }
        }

        void Manager::check_hover(int32_t x, int32_t y)
        {
            auto hovered = false;
            const auto window = ClientEngine::window();
            auto global = window->mapPixelToCoords(sf::Vector2i(x, y));
            
            for (auto screen : s_screens)
            {
                if (!screen->visible())
                {
                    continue;
                }

                for (auto &comp : screen->components())
                {
                    if (comp->enabled())
                    {
                        if (comp->local_view() != nullptr)
                        {
                            auto local = window->mapPixelToCoords(sf::Vector2i(x, y), *comp->local_view());
                            if (!comp->check_hover(local.x, local.y))
                            {
                                continue;
                            }
                        }
                        else if (!comp->check_hover(global.x, global.y))
                        {
                            continue;
                        }

                        change_focus(comp.get());
                        hovered = true;
                        break;
                    }
                }

                if (hovered)
                {
                    break;
                }
            }

            if (!hovered && s_has_focus != nullptr)
            {
                change_focus(nullptr);
            }
        }

        void Manager::check_pressed(int32_t x, int32_t y, sf::Mouse::Button button)
        {
            if (s_has_focus != nullptr)
            {
                s_has_focus->do_click(x, y, button);
            }
        }

        void Manager::change_focus(Component *comp)
        {
            if (s_has_focus != nullptr && s_has_focus != comp)
            {
                s_has_focus->has_focus(false);
            }
            if (comp != nullptr)
            {
                s_last_had_focus = s_has_focus;
            }

            s_has_focus = comp;
            if (comp != nullptr)
            {
                s_has_focus->has_focus(true);
            }
        }
    }
}
