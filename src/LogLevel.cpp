/*
 * @Author: user8361
 * @Date: 2022-07-07 20:09:18
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-07 20:49:19
 * @FilePath: /projects/HLog/src/LogLevel.cpp
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#include "LogLevel.h"

namespace H
{
    LogLevel::Level LogLevel::FromString(const std::string &str)
    {

#define SET_LEVEL(level, v)     \
    if (str == #v)              \
    {                           \
        return LogLevel::level; \
    }
        SET_LEVEL(LogLevel::DEBUG, "debug");
        SET_LEVEL(LogLevel::INFO, "info");
        SET_LEVEL(LogLevel::WARNING, "warning");
        SET_LEVEL(LogLevel::ERROR, "error");
        SET_LEVEL(LogLevel::FATAL, "fatal");

        SET_LEVEL(LogLevel::DEBUG, "DEBUG");
        SET_LEVEL(LogLevel::INFO, "INFO");
        SET_LEVEL(LogLevel::WARNING, "WARNING");
        SET_LEVEL(LogLevel::ERROR, "ERROR");
        SET_LEVEL(LogLevel::FATAL, "FATAL");

        return LogLevel::UNKNOW;

#undef SET_LEVEL
    }

    const char *LogLevel::ToString(LogLevel::Level level)
    {
        switch (level)
        {
#define TO_STRING(level)  \
    case LogLevel::level: \
        return #level;    \
        break;
            TO_STRING(DEBUG);
            TO_STRING(INFO);
            TO_STRING(WARNING);
            TO_STRING(ERROR);
            TO_STRING(FATAL);
#undef TO_STRING
        default:
            return "UNKNOW";
        }

        return "UNKNOW";
    }
}