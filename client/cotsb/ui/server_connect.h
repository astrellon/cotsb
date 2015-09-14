#pragma once

#include "screen.h"

namespace cotsb
{
    namespace ui
    {
        class Button;
        class TextInput; 
        class Label;

        class ServerConnect : public Screen
        {
            public:
                virtual bool init();

                void on_error(const std::string &message);

            private:

                TextInput *_hostname;
                TextInput *_port;
                TextInput *_player_name;
                Button *_connect;
                Button *_back;
                Label *_error_message;
        };

        extern ServerConnect server_connect;
    }
}
