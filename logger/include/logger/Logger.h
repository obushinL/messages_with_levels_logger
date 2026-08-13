#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include <memory>
#include <mutex>
#include <string>
#include <logger/LogLevel.h>
#include <logger/LogTarget.h>

namespace logger
{
    class Logger
    {
    public:
        // Log file init
        // fileName - logger file name
        // defaultLevel - enum LogLevel
        Logger(const std::string& fileName, LogLevel defaultLvl);

        Logger(std::unique_ptr<LogTarget>, LogLevel defaultLvl);

        ~Logger() = default;

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        // Logger init check function
        bool isValid() const;;

        std::string lastError() const;

        // level setter
        void set_level(LogLevel lvl);
        // level getter
        LogLevel get_level() const;
        
        // Log the message, false if error occured
        bool log(const std::string& msg, LogLevel level);
        bool log(const std::string& msg);


        // return log message with level
        bool debug(const std::string& message) { return log(message, LogLevel::Debug); }
        bool info (const std::string& message) { return log(message, LogLevel::Info);  }
        bool error(const std::string& message) { return log(message, LogLevel::Error); }

    private:
        static std::string msgFormat(const std::string& message, LogLevel lvl);
        
        std::unique_ptr<LogTarget> m_target;
        LogLevel                   m_level;
        mutable std::mutex         m_mutex;
        std::string                m_lastErr;
    };
}

#endif