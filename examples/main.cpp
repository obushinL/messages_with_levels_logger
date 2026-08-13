#include <iostream>
#include <memory>

#include "logger/Logger.h"

int main(int argc, char **argv)
{
    using namespace logger;

    const std::string fileName = (argc > 1) ? argv[1] : "log.txt";

    Logger fileLogger(fileName, LogLevel::Info);
    if (!fileLogger.isValid())
    {
        std::cerr << "Logger init error "
                  << fileLogger.lastError() << std::endl;
        return 1;
    }

    fileLogger.set_level(LogLevel::Debug);
    fileLogger.debug("Теперь отладочные сообщения записываются");

    
    return 0;
}
