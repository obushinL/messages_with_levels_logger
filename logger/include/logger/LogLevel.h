#ifndef LOGGER_LOGLEVEL_H
#define LOGGER_LOGLEVEL_H

#include <string>

namespace logger
{
    // enum class added to allow editing levels 
    enum class LogLevel
    {
        Debug, // default 0
        Info,  // default 1
        Error, // default 2
    }; 

    // function enum level -> string
    inline const char *logLevelToString(LogLevel level)
    {
        switch (level)
        {
        // also able to refer to enum Level number 
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Error:
            return "ERROR";
        default:
            return "UNDEFINED LOG";
        }
    }
}

#endif