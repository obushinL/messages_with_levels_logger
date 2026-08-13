#ifndef LOGGER_LOGTARGET_H
#define LOGGER_LOGTARGET_H

#include "logger/LogLevel.h"
#include <string>

// abstract logger interface class 
// for FileLogTarget class and SocketLogTarget 
class LogTarget {
    public:
    virtual ~LogTarget() = default;

    // true if logging success, else is logging failure
    virtual bool write(const std::string& formattedMsg) = 0;

    // Logger status function
    virtual bool isRdy() const = 0;

    // error log text
    virtual std::string lastError() const = 0;
};

#endif