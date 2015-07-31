#pragma once

#include "screen.h"

namespace cotsb
{
    namespace ui
    {
        class Button;
        class TextInput; 

        class ServerConnect : public Screen
        {
            public:
                virtual bool init();

            private:

                TextInput *_hostname;
                TextInput *_port;
                Button *_connect;
                Button *_back;
        };

        extern ServerConnect server_connect;
    }
}
