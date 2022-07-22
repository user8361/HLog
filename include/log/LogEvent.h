/*
 * @Author: user8361
 * @Date: 2022-07-07 21:49:26
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-17 22:20:00
 * @FilePath: /projects/HLog/include/log/LogEvent.h
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#pragma once
#include <memory>
#include <sstream>
#include "utils/Noncopyable.h"
#include "LogLevel.h"
#include <stdarg.h>
namespace H
{
    class Logger;
    class LogEvent : public Noncopyable
    {
    public:
        typedef std::shared_ptr<LogEvent> m_ptr;
        /**
         * @brief Construct a new Log Event object
         *
         * @param logger 日志器
         * @param level 日志级别
         * @param file 文件名
         * @param line 文件行号
         * @param elapse 程序启动依赖的耗时
         * @param thread_id 线程id
         * @param fiber_id 协程id
         * @param time 日志时间（秒）
         * @param thread_name 线程名称
         */
        LogEvent(std::shared_ptr<Logger> logger,
                 LogLevel::Level level,
                 const char *file,
                 int32_t line,
                 uint32_t elapse,
                 uint32_t thread_id,
                 uint32_t fiber_id,
                 uint64_t time,
                 const std::string &thread_name) : m_logger(logger),
                                                   m_level(level),
                                                   m_file(file),
                                                   m_line(line),
                                                   m_elapse(elapse),
                                                   m_thread_id(thread_id),
                                                   m_fiber_id(fiber_id),
                                                   m_time(time),
                                                   m_thread_name(thread_name){};

        /**
         * @brief  返回文件名
         *
         * @return const char*
         */
        const char *GetFile()
        {
            return m_file;
        }

        /**
         * @brief 返回行号
         *
         * @return int32_t
         */
        int32_t GetLine()
        {
            return m_line;
        }

        /**
         * @brief 返回耗时
         *
         * @return uint32_t
         */
        uint32_t GetElapse()
        {
            return m_elapse;
        }

        /**
         * @brief 返回线程id
         *
         * @return uint32_t
         */
        uint32_t GetThreadId()
        {
            return m_thread_id;
        }

        /**
         * @brief 返回协程id
         *
         * @return uint32_t
         */
        uint32_t GetFiberId()
        {
            return m_fiber_id;
        }

        /**
         * @brief 返回时间戳
         *
         * @return uint64_t
         */
        uint64_t GetTime()
        {
            return m_time;
        }

        /**
         * @brief 返回线程名称
         *
         * @return std::string
         */
        std::string GetThreadName()
        {
            return m_thread_name;
        }

        /**
         * @brief  返回日志流内容
         *
         * @return std::stringstream
         */
        std::stringstream *GetStringStream()
        {
            return &m_sstream;
        }

        /**
         * @brief 返回日志器
         *
         * @return std::shared_ptr<Logger>
         */
        std::shared_ptr<Logger> GetLogger()
        {
            return m_logger;
        }

        /**
         * @brief 返回日志级别
         *
         * @return LogLevel::Level
         */
        LogLevel::Level GetLevel()
        {
            return m_level;
        }

        /**
         * @brief 返回日志内容
         *
         * @return std::string
         */
        std::string GetContent()
        {
            return m_sstream.str();
        }

        /**
         * @brief 格式化写入日志内容
         *
         * @param fmt
         * @param ...
         */
        void format(const char *fmt, ...)
        {
            va_list al;
            va_start(al, fmt);
            format(fmt, al);
            va_end(al);
        }
        /**
         * @brief 格式化写入日志内容
         *
         * @param fmt
         * @param al
         */

        void format(const char *fmt, va_list al)
        {
            char *buf = nullptr;
            int len = vasprintf(&buf, fmt, al);
            if (len != -1)
            {
                m_sstream << std::string(buf, len);
                free(buf);
            }
        }

    private:
        const char *m_file;        // 文件名
        int32_t m_line = 0;        //行号
        uint32_t m_elapse = 0;     // 程序启动开始到现在的毫秒
        uint32_t m_thread_id = 0;  // 线程id
        uint32_t m_fiber_id = 0;   // 协程id
        uint64_t m_time = 0;       // 时间戳
        std::string m_thread_name; // 线程名称

        std::stringstream m_sstream;      // 日志内容流
        std::shared_ptr<Logger> m_logger; // 日志器
        LogLevel::Level m_level;          // 日志级别
    };

    /**
     * @brief 日志事件包装器
     *
     */
    class LogEventWrap
    {

    public:
        /**
         * @brief Construct a new Log Event Wrap object
         *
         * @param event
         */
        LogEventWrap(LogEvent::m_ptr event) : m_event(event) {}
        ~LogEventWrap();

        LogEvent::m_ptr getEvent() const { return m_event; };
        /**
         * @brief 返回日志内容流
         *
         * @return std::stringstream
         */
        std::stringstream *getStringStream()
        {
            return m_event->GetStringStream();
        }

    private:
        std::shared_ptr<LogEvent> m_event;
        // LogEvent::m_ptr m_event;
    };
} // H