#include "logging.h"

#include <iostream>

namespace cotsb
{
    Logger logger;
    EndLog endl;

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
    Logger &Logger::operator <<(std::size_t data)
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
    std::map<std::string, std::string> LoggerManager::s_type_to_colours;

    void LoggerManager::init()
    {
        s_type_to_colours["Error"] = "\033[1;31m";
        s_type_to_colours["Info"] = "\033[1;32m";
        s_type_to_colours["Network"] = "\033[1;34m";
        s_type_to_colours["Warning"] = "\033[1;33m";
    }

    void LoggerManager::log(const std::string &type, const std::string &message)
    {
        std::unique_lock<std::mutex> lock_guard(s_log_lock);

        auto find = s_type_to_colours.find(type);
        if (find == s_type_to_colours.end())
        {
            std::cout << type << ": " << message << '\n';
        }
        else
        {
            std::cout << find->second << type << "\033[0m: " << message << '\n';
        }
        // Add more loggers here
    }
    // }}}
}
