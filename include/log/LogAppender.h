/*
 * @Author: user8361
 * @Date: 2022-07-07 15:31:32
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-22 22:05:35
 * @FilePath: /projects/HLog/include/log/LogAppender.h
 * @Description:  输出落地点
 *
 * 定义日志的输出落地点，
 * 目前实现两类：
 *      控制台 StdoutLogAppender
 *      文件日志 FileLogAppender
 * 分别拥有自己的日志级别和日志格式
 * 可以灵活定义不同的输出
 * 主要用于区分日志级别，
 * 将error日志单独输出到一个文件，防止被其他日志淹没掉
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#pragma once

#include <fstream>
// #include "log/Logger.h"
#include "utils/Mutex.h"
#include "log/LogLevel.h"
#include <memory>
namespace H
{
    class Logger;
    class LogEvent; 
    class LogFormatter; 
    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender> m_ptr;
        typedef Spinlock MutexType;

        /**
         * @brief Destroy the Log Append object
         *
         */
        virtual ~LogAppender()
        {
        }
        /**
         * @brief 将日志输出目标的配置转换成YAML STRING
         *
         * @return std::string
         */
        virtual std::string toYamlString() = 0;
        /**
         * @brief 写入日志
         *
         * @param logger 日志器
         * @param level 日志级别
         * @param event 日志事件
         */
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) = 0;

        /**
         * @brief 更改日志格式器
         *
         * @param format
         */
        void setFormatter(std::shared_ptr<LogFormatter> format)
        {
            MutexType::m_lock lock(m_mutex);
            m_formatter = format;
            if (m_formatter)
            {
                m_has_formatter = true;
            }
            else
            {
                m_has_formatter = false;
            }
        };

        /**
         * @brief 返回日志格式器
         *
         * @return std::shared_ptr<LogFormatter>
         */
        std::shared_ptr<LogFormatter> getFormatter()
        {
            MutexType::m_lock lock(m_mutex);
            return m_formatter;
        }

        /**
         * @brief 返回日志级别
         *
         * @return LogLevel::Level
         */
        LogLevel::Level getLevel() const { return m_level; }

        /**
         * @brief 设置日志级别
         *
         * @param level
         */
        void setLevel(LogLevel::Level level) { m_level = level; }

    private:
        friend class Logger;

    protected:
        LogLevel::Level m_level = LogLevel::DEBUG; // 日志级别
        bool m_has_formatter = false;              // 是否有自己的日志格式器
        MutexType m_mutex;                         // mutex
        std::shared_ptr<LogFormatter> m_formatter; // 日志格式器
    };

    /**
     * @brief 输出到控制台的 Appender
     *
     */
    class StdoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender> m_ptr;
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override;
        std::string toYamlString() override;
        ~StdoutLogAppender() {}
    };

    /**
     * @brief  输出到文件的 Appender
     *
     */
    class FileLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<FileLogAppender> m_ptr;
        /**
         * @brief Construct a new File Log Appender object
         *
         * @param filename
         */
        FileLogAppender(const std::string &filename);
        /**
         * @brief 输出日志
         *
         * @param logger 日志器
         * @param level 日志级别
         * @param event 日志事件
         */
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override;
        /**
         * @brief 将日志输出目标的配置转换为 Yaml String
         *
         * @return std::string
         */
        std::string toYamlString() override;

        /**
         * @brief 重新打开日志文件
         *
         * @return true
         * @return false
         */
        bool reopen();
        ~FileLogAppender() {}

    private:
        std::string m_file_name;     // 文件路径
        std::ofstream m_file_stream; // 文件流
        uint64_t m_last_time = 0;    //上次重新打开文件的时间
    };
}
