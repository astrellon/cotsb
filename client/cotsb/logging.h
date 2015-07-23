#pragma once

#include <sstream>
#include <vector>
#include <memory>
#include <stdint.h>

namespace cotsb
{
    class ILogger
    {
        public:
            virtual void log(const std::string &type, const std::string &message) = 0;
    };

    class LogType
    {
        public:
            inline LogType() {}
            inline LogType(const std::string &type) :
                _type(type)
            {
            }
            inline LogType(const char *type) :
                _type(type)
            {
            }

            inline const std::string &type() const
            {
                return _type;
            }

        private:
            std::string _type;
    };

    class EndLog
    {

    };

    class Logger
    {
        public:
            Logger& operator <<(bool                data);
            Logger& operator <<(int8_t              data);
            Logger& operator <<(uint8_t             data);
            Logger& operator <<(int16_t             data);
            Logger& operator <<(uint16_t            data);
            Logger& operator <<(int32_t             data);
            Logger& operator <<(uint32_t            data);
            Logger& operator <<(float               data);
            Logger& operator <<(double              data);
            Logger& operator <<(const char*         data);
            Logger& operator <<(const std::string&  data);
            Logger& operator <<(const LogType&      type);
            Logger& operator <<(const EndLog&      type);

            void flush();

        private:
            std::stringstream _buffer;
            LogType _type;

    };

    class LoggerManager
    {
        public:
            static void init();

            static void log(const std::string &type, const std::string &message);

        private:
            static std::vector<std::unique_ptr<ILogger> > s_loggers;

    };

    static Logger logger;
    static EndLog endl;
}
