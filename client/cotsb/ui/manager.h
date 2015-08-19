#pragma once

#include <vector>
#include <memory>

#include "component.h"

namespace cotsb
{
    namespace ui
    {
        class Screen;

        class Manager 
        {
        public:
            static bool init();
            static void deinit();

            static void add_component(Component *component);
            static void remove_component(Component *component);

            static void focus(Component *component);
            static Component *focus();

            static void update(float dt);
            static void draw(sf::RenderTarget &target, sf::RenderStates states);

            static void process_event(const sf::Event &event);

            typedef std::vector<Screen *> ScreenList;
            static const ScreenList *screen();

        private:
            static ScreenList s_screens;
            static Component *s_has_focus;
            static Component *s_last_had_focus;

            static void check_hover(int32_t x, int32_t y);
            static void check_pressed(int32_t x, int32_t y, sf::Mouse::Button button);

            static void change_focus(Component *comp);
        };
    }
}
