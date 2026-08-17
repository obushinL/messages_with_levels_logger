#ifndef APP_MESSAGE_H
#define APP_MESSAGE_H

#include <string>
#include "logger/LogLevel.h"

struct LogMessage {
    std::string      text;
    logger::LogLevel level;
};

#endif