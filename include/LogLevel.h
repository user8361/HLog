/*
 * @Author: user8361
 * @Date: 2022-07-07 20:03:51
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-12 14:48:08
 * @FilePath: /projects/HLog/include/LogLevel.h
 * @Description: 日志级别
 *
 * 日志级别：
 *
 *      FromString 从文本输入日志级别
 *      ToString将日志级别输出为文本
 *
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#pragma once
#include <string>
namespace H
{

    class LogLevel
    {
    public:
        enum Level
        {
            UNKNOW = 0,
            DEBUG = 1,
            INFO = 2,
            WARNING = 3,
            ERROR = 4,
            FATAL = 5
        };

        /**
         * @brief 将文本转换成日志级别
         *
         *
         * @param str 日志级别文本
         * @return LogLevel::Level
         */
        static LogLevel::Level FromString(const std::string &str);

        /**
         * @brief 将日志级别转换成文本输出
         *
         * @param level 日志级别
         * @return const char*
         */
        static const char *ToString(LogLevel::Level level);
    };
}
