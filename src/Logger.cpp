#include "logger/Logger.h"
#include "logger/FileLogTarget.h"

#include <ctime>
#include <cstdio>

//#define _WIN32;

namespace logger
{
    Logger::Logger(const std::string &fileName, LogLevel lvl)
        : m_target(new FileLogTarget(fileName)), m_level(lvl)
    {

        // if no targer or logger is not ready for logging
        if (!m_target || !m_target->isRdy())
        {
            // if no target then rewrite with last error message,
            // otherwise a new error text is written
            m_lastErr = m_target ? m_target->lastError() : std::string("No log receiver");
        }
    }

    Logger::Logger(std::unique_ptr<LogTarget> target, LogLevel defaultLevel)
        : m_target(std::move(target)), m_level(defaultLevel)
    {
        if (!m_target || !m_target->isRdy())
        {
            m_lastErr = m_target ? m_target->lastError()
                                 : std::string("No message receiver");
        }
    }

    bool Logger::isValid() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        return m_target && m_target->isRdy();
    }

    std::string Logger::lastError() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_lastErr.empty())
        {
            return m_lastErr;
        }
        return m_target ? m_target->lastError() : std::string();
    }

    void Logger::set_level(LogLevel level)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_level = level;
    }

    LogLevel Logger::get_level() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_level;
    }

    std::string Logger::msgFormat(const std::string &message, LogLevel level)
    {
        // get current time
        std::time_t now = std::time(nullptr);
        std::tm tmBuf;

        //_______________________________________________________________
        /*

        USE localtime_r for Lunix, macOS, Unix

        */
#ifdef _WIN32
        ::localtime_s(&tmBuf, &now);
#else
        ::localtime_r(&now, &tmBuf);
#endif

        /*

        USE localtime_s for Windows

        */

        //_______________________________________________________________

        // formatting time to string "YYYY-MM-DD HH:MM:SS"
        char timeStr[32];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tmBuf);

        std::string result;
        result.reserve(message.size() + 40);
        result += timeStr;
        result += " [";
        result += logLevelToString(level);
        result += "] ";
        result += message;

        return result; // Result string "0000-00-00 00:00:00 [Debug] Message"
    }

    bool Logger::log(const std::string &message, LogLevel msgLevel)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_target || !m_target->isRdy())
        {
            m_lastErr = "Logger isn't initialized";
            return false;
        }

        // Message level filtering
        if (static_cast<int>(msgLevel) < static_cast<int>(m_level))
        {
            return false; 
        }

        const std::string formatted = msgFormat(message, msgLevel);
        if (!m_target->write(formatted))
        {
            m_lastErr = m_target->lastError();
            return false;
        }
        return true;
    }

    bool Logger::log(const std::string &message)
    {
        return log(message, get_level());
    }

}