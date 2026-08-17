#include <iostream>
#include "logger/Logger.h"

int main(int argc, char** argv) {
    using namespace logger;
    const std::string file = (argc > 1) ? argv[1] : "log.txt";

    Logger log(file, LogLevel::Info);
    if (!log.isValid()) {
        std::cerr << "Init error: " << log.lastError() << "\n";
        return 1;
    }
    log.info("Пример: библиотека подключена и работает");
    return 0;
}