/*
 * @Author: user8361
 * @Date: 2022-06-28 21:03:20
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-11 15:34:06
 * @FilePath: /projects/HLog/src/Logger.cpp
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */
#include "log/Logger.h"
#include "log/LogFormatter.h"
namespace H
{

    Logger::Logger(const std::string &name) : m_name(name), m_level(LogLevel::DEBUG)
    {

        //默认格式 "%d{%Y-%m-%d %H:%M:%S} thread_id thread_name fiber_id [log_level] [log_name] file_name:line msg \n"
        //                                      年 - 月 - 日 时 : 分 : 秒 线程名称 线程id  协程id[日志级别][日志名称] 文件名称 : 行号 消息 \n
        m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%N%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    }

    void Logger::setFormatter(LogFormatter::m_ptr formatter)
    {
        MutexType::m_lock lock(m_mutex);
        m_formatter = formatter;

        for (auto &i : m_appenders)
        {
            MutexType::m_lock ll(i->m_mutex);
            if (!i->m_has_formatter)
            {
                i->m_formatter = m_formatter;
            }
        }
    }
    void Logger::setFormatter(const std::string &format)
    {
        MutexType::m_lock lock(m_mutex);
        // m_formatter->format(this,LogLevel::DEBUG,)
        LogFormatter::m_ptr formatter(new LogFormatter(format));
        if (formatter->isError())
        {
            std::cout << "Logger setFormatter name = " << m_name
                      << "value = " << format << " is invalid formatter." << std::endl;
            return;
        }
        setFormatter(formatter);
    }
    LogFormatter::m_ptr Logger::getFormatter()
    {
        MutexType::m_lock lock(m_mutex);
        return m_formatter;
    }
    std::string Logger::toYamlString()
    {
        MutexType::m_lock lock(m_mutex);
        YAML::Node node;

        node["name"] = m_name;
        if (m_level != LogLevel::UNKNOW)
        {
            node["level"] = LogLevel::ToString(m_level);
        }
        if (m_formatter)
        {
            node["formatter"] = m_formatter->GetPattern();
        }

        for (auto &i : m_appenders)
        {
            node["appenders"].push_back(YAML::Load(i->toYamlString()));
        }
        std::stringstream sstream;
        sstream << node;
        return sstream.str();
    }

    void Logger::addApender(LogAppender::m_ptr appender)
    {
        MutexType::m_lock lock(m_mutex);
        if (!appender->getFormatter())
        {
            MutexType::m_lock ll(appender->m_mutex);
            appender->m_formatter = m_formatter;
        }
        m_appenders.push_back(appender);
    }

    void Logger::delAppender(LogAppender::m_ptr appender)
    {
        MutexType::m_lock lock(m_mutex);
        for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it)
        {
            if (*it == appender)
            {
                m_appenders.erase(it);
                break;
            }
        }
    }

    void Logger::clearAppender()
    {
        MutexType::m_lock lock(m_mutex);
        m_appenders.clear();
    }

    void Logger::log(LogLevel::Level level, LogEvent::m_ptr event)
    {
        if (level >= m_level)
        {
            auto self = shared_from_this();
            MutexType::m_lock lock(m_mutex);
            if (!m_appenders.empty())
            {
                for (auto &i : m_appenders)
                {
                    i->log(self, level, event);
                }
            }
            else
            {
                m_root->log(level, event);
            }
        }
    }

    void Logger::debug(LogEvent::m_ptr event)
    {
        log(LogLevel::DEBUG, event);
    }
    void Logger::info(LogEvent::m_ptr event)
    {
        log(LogLevel::INFO, event);
    }
    void Logger::warning(LogEvent::m_ptr event)
    {
        log(LogLevel::WARNING, event);
    }
    void Logger::error(LogEvent::m_ptr event)
    {
        log(LogLevel::ERROR, event);
    }
    void Logger::fatal(LogEvent::m_ptr event)
    {
        log(LogLevel::FATAL, event);
    }
}