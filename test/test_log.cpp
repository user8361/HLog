/*
 * @Author: user8361
 * @Date: 2022-07-13 12:30:29
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-17 22:41:41
 * @FilePath: /projects/HLog/test/test_log.cpp
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */
#include <iostream>
#include "log/Logger.h"
#include "log/LogAppender.h"
#include "log/LogFormatter.h"
#include "log/LogEvent.h"
#include "utils/Utils.h"
#include "utils/Fiber.h"
#include <time.h>

int main()
{

    H::Logger::m_ptr logger(new H::Logger);
    logger->addApender(H::LogAppender::m_ptr(new H::StdoutLogAppender));

    H::FileLogAppender::m_ptr file_appender(new H::FileLogAppender("./log.txt"));
    H::LogFormatter::m_ptr fmt(new H::LogFormatter("%d%T%p%T%m%n"));

    file_appender->setFormatter(fmt);
    file_appender->setLevel(H::LogLevel::ERROR);
    logger->addApender(file_appender);
    H_LOG_INFO(logger) << "test macro";
    H_LOG_ERROR(logger) << "test macro error";

    // H::LogEvent::m_ptr event(new H::LogEvent(logger, H::LogLevel::INFO, __FILE__, __LINE__, 0, H::GetThreadId(), H::GetFiberId(), time(0), "test"));
    // *event->GetStringStream() << "Hello";

    // logger->log(H::LogLevel::INFO, event);
    // std::cout << "Hello H log" << std::endl;

    // H_LOG_FMT_LEVEL() ;

    H_LOG_INFO(logger) << "test";
    H_LOG_FMT_ERROR(logger, "test macro fmt errors %s", "aa");
    // H_LOG_INFO(l) << "xxx";
    return 0;
}
