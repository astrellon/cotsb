#pragma once

#include <sstream>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include <stdint.h>

namespace cotsb
{
    class ILogger
    {
        public:
            virtual void log(const std::string &type, const std::string &message) = 0;
    };

    class EndLog { };

    // Logger {{{
    class Logger
    {
        public:
            Logger& operator <<(bool               data);
            Logger& operator <<(int8_t             data);
            Logger& operator <<(uint8_t            data);
            Logger& operator <<(int16_t            data);
            Logger& operator <<(uint16_t           data);
            Logger& operator <<(int32_t            data);
            Logger& operator <<(uint32_t           data);
            Logger& operator <<(int64_t            data);
            Logger& operator <<(uint64_t           data);
            Logger& operator <<(float              data);
            Logger& operator <<(double             data);
            Logger& operator <<(const char*        data);
            Logger& operator <<(const std::string& data);
            Logger& operator <<(const EndLog&      type);

            Logger& operator %(const char          *type);
            Logger& operator %(const std::string   &type);

            void flush();

        private:

            struct TypeBufferPair
            {
                std::stringstream buffer;
                std::string type;
            };

            std::map<std::thread::id, TypeBufferPair> _buffers;
    };
    // }}}

    // LoggerManager {{{
    class LoggerManager
    {
        public:
            static void init();

            static void log(const std::string &type, const std::string &message);

        private:
            static std::vector<std::unique_ptr<ILogger> > s_loggers;
            static std::mutex s_log_lock;

    };
    // }}}

    static Logger logger;
    static EndLog endl;
}