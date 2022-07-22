/*
 * @Author: user8361
 * @Date: 2022-07-09 15:56:45
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-22 22:07:48
 * @FilePath: /projects/HLog/src/log/LogAppender.cpp
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#include "log/LogAppender.h"
#include "log/LogFormatter.h"
#include "utils/Utils.h"
#include <yaml-cpp/yaml.h>
namespace H
{
    FileLogAppender::FileLogAppender(const std::string &filename) : m_file_name(filename)
    {
        reopen();
    }
    void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event)
    {
        if (level >= m_level)
        {
            uint64_t now = event->GetTime();
            if (now >= (m_last_time + 3))
            {
                reopen();
                m_last_time = now;
            }
            MutexType::m_lock lock(m_mutex);
            // if(!(m_filestream << m_formatter->format(logger, level, event))) {
            if (!m_formatter->format(m_file_stream, logger, level, event))
            {
                std::cout << "error" << std::endl;
            }
        }
    }

    std::string FileLogAppender::toYamlString()
    {
        MutexType::m_lock lock(m_mutex);
        YAML::Node node;
        node["type"] = "FileLogAppender";
        node["file"] = m_file_name;
        if (m_level != LogLevel::UNKNOW)
        {
            node["level"] = LogLevel::ToString(m_level);
        }
        if (m_has_formatter && m_formatter)
        {
            node["formatter"] = m_formatter->GetPattern();
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

    bool FileLogAppender::reopen()
    {
        MutexType::m_lock lock(m_mutex);
        if (m_file_stream)
        {
            m_file_stream.close();
        }

        return FSUtil::OpenForWrite(m_file_stream, m_file_name, std::ios::app);
    }

    void StdoutLogAppender::log(Logger::m_ptr logger, LogLevel::Level level, LogEvent::m_ptr event)
    {
        if (level >= m_level)
        {
            MutexType::m_lock lock(m_mutex);
            m_formatter->format(std::cout, logger, level, event);
        }
    }

    std::string StdoutLogAppender::toYamlString()
    {
        MutexType::m_lock lock(m_mutex);
        YAML::Node node;
        node["type"] = "StdoutLogAppender";
        if (m_level != LogLevel::UNKNOW)
        {
            node["level"] = LogLevel::ToString(m_level);
        }

        if (m_has_formatter && m_formatter)
        {
            node["formmater"] = m_formatter->GetPattern();
        }
        std::stringstream ss;
        ss << node;

        return ss.str();
    }
}