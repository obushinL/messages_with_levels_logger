// app/main.cpp
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>

#include "logger/Logger.h"
#include "message.h"        
#include "thread_queue.h"   

using namespace logger;

bool parseLevel(const std::string& raw, LogLevel& out) {
    std::string s = raw;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    if (s == "debug") { out = LogLevel::Debug; return true; }
    if (s == "info")  { out = LogLevel::Info;  return true; }
    if (s == "error") { out = LogLevel::Error; return true; }
    return false;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0]
                  << " <файл_журнала> <уровень: Debug|Info|Error>\n";
        return 1;
    }

    const std::string fileName = argv[1];
    LogLevel defaultLevel;
    if (!parseLevel(argv[2], defaultLevel)) {
        std::cerr << "Неверный уровень: " << argv[2] << "\n";
        return 1;
    }

    Logger logger(fileName, defaultLevel);
    if (!logger.isValid()) {
        std::cerr << "Logger init error: " << logger.lastError() << "\n";
        return 1;
    }

    ThreadSafeQueue<LogMessage> queue;   

    // write thread
    std::thread writer([&logger, &queue] {
        LogMessage msg;
        while (queue.waitAndPop(msg)) {
            bool ok = logger.log(msg.text, msg.level);
            if (!ok && !logger.isValid())
                std::cerr << "[Ошибка] " << logger.lastError() << "\n";
        }
    });

    std::cout << "Файл: " << fileName
              << ", уровень по умолчанию: " << logLevelToString(defaultLevel) << "\n"
              << "Вводите сообщение (в конце можно указать уровень). Выход: exit\n\n";

    // input cycle
    std::string line;
    while (std::cout << "> ", std::getline(std::cin, line)) {
        if (line == "exit" || line == "quit") break;
        if (line.find_first_not_of(" \t") == std::string::npos) continue;

        LogMessage msg;
        msg.level = defaultLevel;
        msg.text  = line;

        // get level
        size_t sp = line.find_last_of(" \t");
        if (sp != std::string::npos) {
            LogLevel lvl;
            if (parseLevel(line.substr(sp + 1), lvl)) {
                msg.level = lvl;
                msg.text  = line.substr(0, sp);
            }
        }

        queue.push(std::move(msg));  
    }

    queue.stop();
    writer.join();
    std::cout << "Готово.\n";
    return 0;
}