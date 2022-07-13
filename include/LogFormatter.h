/*
 * @Author: user8361
 * @Date: 2022-07-07 23:12:35
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-13 12:44:59
 * @FilePath: /projects/HLog/include/LogFormatter.h
 * @Description: 日志格式器
 *
 * 日志格式，通过字符串自定义日志的格式，模仿 printf格式，自由定义日志格式
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#pragma once
// #include "Logger.h"
#include <memory>
#include <string>
#include <vector>
#include "Logger.h"
#include "LogLevel.h"
#include "LogEvent.h"
namespace H
{
    class Logger;
    class LogEvent;
    class LogFormatter
    {
    public:
        /**
         * @brief 日志内容格式化
         *
         */
        class FormatItem
        {
        public:
            typedef std::shared_ptr<FormatItem> m_ptr;
            /**
             * @brief Destroy the Format Item object
             *
             */
            virtual ~FormatItem() {}

            /**
             * @brief 格式化日志到输出流
             *
             * @param os 日志输出流
             * @param logger 日志器
             * @param level 日志级别
             * @param event 日志事件
             */
            virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) = 0;
        };

    public:
        typedef std::shared_ptr<LogFormatter> m_ptr;

        /**
         * @brief Construct a new Log Formatter object
         *
         * @param pattern 模板格式

         * 格式如下

         *  默认格式 "%d{%Y-%m-%d %H:%M:%S} thread_id thread_name fiber_id [log_level] [log_name] file_name:line msg \n"
                         年-月-日 时:分:秒 线程id 线程名称 协程id [日志级别] [日志名称] 文件名称:行号 消息 \n
         */
        LogFormatter(const std::string &pattern);

        /**
         * @brief 返回格式化日志文本
         *
         * @param logger 日志器
         * @param level 日志级别
         * @param event 日志事件
         * @return std::string
         */
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event);
        std::ostream &format(std::ostream &ofs, std::shared_ptr<Logger> Logger, LogLevel::Level level, std::shared_ptr<LogEvent> event);

    public:
        /**
         * @brief 初始化，解析日志模板
         *
         */
        void init();

        /**
         * @brief 是否有错
         *
         * @return true
         * @return false
         */
        bool isError() const
        {
            return m_error;
        }

        /**
         * @brief 返回日志模板
         *
         * @return const std::string
         */
        const std::string GetPattern() const
        {
            return m_pattern;
        }

    private:
        std::string m_pattern;                  // 日志格式模板
        std::vector<FormatItem::m_ptr> m_items; // 日志内容格式化
        bool m_error = false;                   // 是否有错
    };

    /**
     * @brief 消息日志格式化
     *
     */
    class MessageFormatItem : public LogFormatter::FormatItem
    {
    public:
        MessageFormatItem(const std::string &str = "") {}

        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            os << event->GetContent();
        }
    };

    /**
     * @brief 日志级别格式化
     *
     */
    class LevelFormatItem : public LogFormatter::FormatItem
    {
    public:
        LevelFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            os << LogLevel::ToString(level);
        }
    };

    class ElapseFormatItem : public LogFormatter::FormatItem
    {
    public:
        ElapseFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            os << event->GetElapse();
        }
    };
    class NameFormatItem : public LogFormatter::FormatItem
    {
    public:
        NameFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            // os << event->GetLogger();
            os << event->GetLogger()->getName();
        }
    };

    class ThreadIdFormatItem : public LogFormatter::FormatItem
    {
    public:
        ThreadIdFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            os << event->GetThreadId();
        }
    };

    class NewLineFormatItem : public LogFormatter::FormatItem
    {
    public:
        NewLineFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            os << std::endl;
        }
    };

    class DateTimeFormatItem : public LogFormatter::FormatItem
    {
    public:
        DateTimeFormatItem(const std::string &format = "%Y-%m-%d %H:%M:%S") : m_format(format)
        {
            if (m_format.empty())
            {
                m_format = "%Y-%m-%d %H:%M:%S";
            }
        }
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            struct tm tm;
            time_t time = event->GetTime();
            localtime_r(&time, &tm);
            char buf[64];
            strftime(buf, sizeof(buf), m_format.c_str(), &tm);
            os << buf;
        }

    private:
        std::string m_format;
    };

    class FilenameFormatItem : public LogFormatter::FormatItem
    {
    public:
        FilenameFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            os << event->GetFile();
        }
    };

    class LineFormatItem : public LogFormatter::FormatItem
    {
    public:
        LineFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            os << event->GetLine();
        }
    };

    class TabFormatItem : public LogFormatter::FormatItem
    {
    public:
        TabFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            // os << event->GetLine();
            os << "\t";
        }
    };
    class FiberIdFormatItem : public LogFormatter::FormatItem
    {
    public:
        FiberIdFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            os << event->GetFiberId();
        }
    };
    class ThreadNameFormatItem : public LogFormatter::FormatItem
    {
    public:
        ThreadNameFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            os << event->GetThreadName();
        }
    };

    class StringFormatItem : public LogFormatter::FormatItem
    {
    public:
        StringFormatItem(const std::string &str = "") : m_string(str) {}
        void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, std::shared_ptr<LogEvent> event) override
        {
            os << m_string;
        }

    private:
        std::string m_string;
    };
}
