/*
 * @Author: user8361
 * @Date: 2022-06-28 21:03:13
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-12 15:25:33
 * @FilePath: /projects/HLog/include/Logger.h
 * @Description: 日志器
 *
 * 对外使用的类，输入的日志级别大于等于 Logger的日志才能被真正写入
 * 可以有多个Logger，不同的logger，记录不同类型的日志
 * 如：将系统框架日志和业务逻辑日志分离
 */
#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <yaml-cpp/yaml.h>

#include "Mutex.h"
#include "LogLevel.h"
#include "LogEvent.h"
// #include "LogFormatter.h"
#include "LogAppender.h"
#include <functional>

namespace H
{
    class LogAppender;
    class LogFormatter;
    // 当希望将this指针托管给shared_ptr时，
    // 类需要继承自std::enable_shared_from_this，
    // 并且从shared_from_this()中获得shared_ptr指针。
    class Logger : public std::enable_shared_from_this<Logger>
    {
    public:
        typedef std::shared_ptr<Logger> m_ptr;
        typedef Spinlock MutexType;

    private:
        friend class LoggerManager;

    public:
        /**
         * @brief Construct a new Logger object
         *
         * @param name 日志器名称
         */
        Logger(const std::string &name = "root");
        // ~Logger();

        /**
         * @brief 写日志
         *
         * @param level 日志级别
         * @param event 日志事件
         */
        void log(LogLevel::Level level, LogEvent::m_ptr event);

        /**
         * @brief debug级别日志
         *
         * @param event 日志事件
         */
        void debug(LogEvent::m_ptr event);

        /**
         * @brief info级别日志
         *
         * @param event
         */
        void info(LogEvent::m_ptr event);

        /**
         * @brief warning级别日志
         *
         * @param event 日志事件
         */
        void warning(LogEvent::m_ptr event);

        /**
         * @brief error日志级别
         *
         * @param event 日志事件
         */
        void error(LogEvent::m_ptr event);

        /**
         * @brief fatal级别日志
         *
         * @param event 日志事件
         */
        void fatal(LogEvent::m_ptr event);

        /* ==============================================================  */

        /**
         * @brief 添加日志目标
         *
         * @param appender
         */
        void addApender(std::shared_ptr<LogAppender> appender);

        /**
         * @brief 删除日志目标
         *
         * @param appender
         */
        void delAppender(std::shared_ptr<LogAppender> appender);

        /**
         * @brief 清空日志目标
         *
         */
        void clearAppender();

        /* ==============================================================  */
        /**
         * @brief 返回日志级别
         *
         * @return LogLevel::Level
         */
        LogLevel::Level getLevel() const
        {
            return m_level;
        }

        /**
         * @brief 设置日志级别
         *
         * @param level
         */
        void setLevel(LogLevel::Level level)
        {
            m_level = level;
        }
        /* ==============================================================  */
        /**
         * @brief 返回日志名称
         *
         * @return const std::string&
         */
        const std::string &getName() const
        {
            return m_name;
        }

        /**
         * @brief 设置日志格式器
         *
         * @param format
         */
        void setFormatter(std::shared_ptr<LogFormatter> format);

        /**
         * @brief 设置日志格式模板
         *
         * @param format
         */
        void setFormatter(const std::string &format);

        /**
         * @brief 返回日志格式器
         *
         * @return LogFormatter::m_ptr
         */
        std::shared_ptr<LogFormatter> getFormatter();

        /**
         * @brief 将日志器的配置转换成 yaml string
         *
         * @return std::string
         */
        std::string toYamlString();

    private:
        std::string m_name;                                  // 日志名称
        LogLevel::Level m_level;                             // 日志级别
        MutexType m_mutex;                                   // mutex
        std::shared_ptr<LogFormatter> m_formatter;           // 日志格式器
        Logger::m_ptr m_root;                                // 主日志器
        std::list<std::shared_ptr<LogAppender>> m_appenders; // 日志目标集合
    };
}
