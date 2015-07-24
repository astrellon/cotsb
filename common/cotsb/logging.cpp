#include "logging.h"

#include <iostream>

namespace cotsb
{
    // Logger {{{
    Logger &Logger::operator <<(bool data)
    {
        _buffers[std::this_thread::get_id()].buffer << (data ? "true" : "false");
        return *this;
    }
    Logger &Logger::operator <<(int8_t data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(uint8_t data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(int16_t data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(uint16_t data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(int32_t data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(uint32_t data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(int64_t data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(uint64_t data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(float data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(double data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    
    Logger &Logger::operator <<(const char *data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
        return *this;
    }
    Logger &Logger::operator <<(const std::string &data)
    {
        _buffers[std::this_thread::get_id()].buffer << data;
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
        _buffers[std::this_thread::get_id()].type = std::string(type);
        return *this;
    }
    Logger &Logger::operator%(const std::string &type)
    {
        flush();
        _buffers[std::this_thread::get_id()].type = type;
        return *this;
    }

    void Logger::flush()
    {
        auto &pair = _buffers[std::this_thread::get_id()];
        if (!pair.type.empty() || !pair.buffer.str().empty())
        {
            LoggerManager::log(pair.type, pair.buffer.str());
            pair.type.clear();
            pair.buffer.str("");
        }
    }
    // }}}

    // LoggerManager {{{
    std::vector<std::unique_ptr<ILogger> > LoggerManager::s_loggers;
    std::mutex LoggerManager::s_log_lock;
    void LoggerManager::init()
    {

    }
    void LoggerManager::log(const std::string &type, const std::string &message)
    {
        std::unique_lock<std::mutex> lock_guard(s_log_lock);
        std::cout << type << ": " << message << '\n';
        // Add more loggers here
    }
    // }}}
}
