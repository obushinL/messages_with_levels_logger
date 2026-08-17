#include "logger/FileLogTarget.h"

namespace logger
{
    FileLogTarget::FileLogTarget(const std::string &fileName) : m_fileName(fileName)
    {
        // open file m_fileName, open for output, append text
        m_stream.open(m_fileName, std::ios::out | std::ios::app);

        if (m_stream.is_open())
            m_lastError.clear();
        else
            m_lastError = "Couldn't open file " + m_fileName;
    }

    FileLogTarget::~FileLogTarget()
    {
        if (m_stream.is_open())
        {
            m_stream.flush();
            m_stream.close();
        }
    }

    bool FileLogTarget::write(const std::string &msgForm)
    {
        if (!m_stream.is_open())
        {
            m_lastError = "Logger file is not opened";
            return false;
        }
        m_stream << msgForm << '\n';
        // Immediate text push
        m_stream.flush();
        if (m_stream.fail())
        {
            m_lastError = "Error logging into " + m_fileName;
            return false;
        }
        return true;
    }

    bool FileLogTarget::isRdy() const
    {
        return m_stream.is_open();
    }

    std::string FileLogTarget::lastError() const
    {
        return m_lastError;
    }

}