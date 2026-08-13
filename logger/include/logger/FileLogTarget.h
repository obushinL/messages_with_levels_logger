#ifndef LOGGER_FILELOGTARGET_H
#define LOGGER_FILELOGTARGET_H

#include <fstream>
#include <string>
#include "logger/LogTarget.h"

namespace logger
{
    class FileLogTarget : public LogTarget
    {
    public:
        // explicit disables implicit conversion
        // good for class constructors
        explicit FileLogTarget(const std::string &fileName);
        ~FileLogTarget() override;

        // Copy constructor and assignment operator are deleted to prevent
        // multiple objects from owning the same file resource
        FileLogTarget(const FileLogTarget &) = delete;
        FileLogTarget &operator=(const FileLogTarget &) = delete;

        bool write(const std::string &formattedMessage) override;
        bool isRdy() const override;
        std::string lastError() const override;

    private:
        std::string   m_fileName;
        std::ofstream m_stream;
        std::string   m_lastError;
    };
}

#endif