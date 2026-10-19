#pragma once

#include <mutex>
#include <print>

class Logger
{
public:
    static Logger& getInstance()
    {
        static Logger instance;
        return instance;
    }

    template <typename... Args>
    void log(const char* level, Args&&... args)
    {
        std::lock_guard<std::mutex> lock(_logMutex);
        std::print("[{}] ", level);
        (std::print("{}", std::forward<Args>(args)), ...);
        std::print("\n");
    }

private:
    Logger() = default;

    std::mutex _logMutex;
};

#define LOG_ERROR(...) Logger::getInstance().log("ERROR", __VA_ARGS__)
#define LOG_INFO(...) Logger::getInstance().log("INFO", __VA_ARGS__)
#ifdef NDEBUG
#define LOG_DEBUG(...)
#else
#define LOG_DEBUG(...) Logger::getInstance().log("DEBUG", __VA_ARGS__)
#endif
