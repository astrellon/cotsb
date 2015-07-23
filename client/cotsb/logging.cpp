#include "logging.h"

#include <iostream>

namespace cotsb
{
    Logger &Logger::operator <<(bool data)
    {
        _buffer << (data ? "true" : "false");
        return *this;
    }
    Logger &Logger::operator <<(int8_t data)
    {
        _buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(uint8_t data)
    {
        _buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(int16_t data)
    {
        _buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(uint16_t data)
    {
        _buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(int32_t data)
    {
        _buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(uint32_t data)
    {
        _buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(float data)
    {
        _buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(double data)
    {
        _buffer << data;
        return *this;
    }
    
    Logger &Logger::operator <<(const char *data)
    {
        _buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(const std::string &data)
    {
        _buffer << data;
        return *this;
    }
    Logger &Logger::operator<<(const EndLog &end)
    {
        flush();
        return *this;
    }

    Logger &Logger::operator%(const char *type)
    {
        flush();
        _type = std::string(type);
        return *this;
    }
    Logger &Logger::operator%(const std::string &type)
    {
        flush();
        _type = type;
        return *this;
    }

    void Logger::flush()
    {
        if (!_type.empty() || !_buffer.str().empty())
        {
            LoggerManager::log(_type, _buffer.str());
            _type.clear();
            _buffer.str("");
        }
    }

    void LoggerManager::init()
    {

    }
    void LoggerManager::log(const std::string &type, const std::string &message)
    {
        std::cout << type << ": " << message << '\n';
    }
}
