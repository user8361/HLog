/*
 * @Author: user8361
 * @Date: 2022-07-09 16:23:56
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-17 22:34:23
 * @FilePath: /projects/HLog/include/utils/Utils.h
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#pragma once
#include <iostream>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include "utils/Fiber.h"
#include "log/LoggerManager.h"

/**
 * @brief 使用流的方式将日志级别写入到logger中
 *
 */
#define H_LOG_LEVEL(logger, level)                                                                          \
    if ((logger->getLevel()) <= level)                                                                      \
    *(H::LogEventWrap(std::shared_ptr<H::LogEvent>(new H::LogEvent(logger, level,                           \
                                                                   __FILE__, __LINE__, 0, H::GetThreadId(), \
                                                                   H::GetFiberId(), time(0), "test")))      \
          .getStringStream())

/**
 * @brief 获取name的日志器
 *
 */
#define H_LOG_NAME(name) H::LoggerMgr::GetInstance()->GetLogger(name)
/**
 * @brief  使用流式方式将日志级别DEBUG写入到logger
 *
 */
#define H_LOG_DEBUG(logger) H_LOG_LEVEL(logger, H::LogLevel::DEBUG)

/**
 * @brief  使用流式方式将日志级别INFO写入到logger
 *
 */
#define H_LOG_INFO(logger) H_LOG_LEVEL(logger, H::LogLevel::INFO)

/**
 * @brief  使用流式方式将日志级别WARNING写入到logger
 *
 */
#define H_LOG_WARNING(logger) H_LOG_LEVEL(logger, H::LogLevel::WARNING)

/**
 * @brief  使用流式方式将日志级别ERROR写入到logger
 *
 */
#define H_LOG_ERROR(logger) H_LOG_LEVEL(logger, H::LogLevel::ERROR)

/**
 * @brief  使用流式方式将日志级别FATAL写入到logger
 *
 */
#define H_LOG_FATAL(logger) H_LOG_LEVEL(logger, H::LogLevel::FATAL)

#define H_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if (logger->getLevel() <= level)             \
    H::LogEventWrap(H::LogEvent::m_ptr(new H::LogEvent(logger, level, __FILE__, __LINE__, 0, H::GetThreadId(), H::GetFiberId(), time(0), "name"))).getEvent()->format(fmt, __VA_ARGS__)

#define H_LOG_FMT_DEBUG(logger, fmt, ...) H_LOG_FMT_LEVEL(logger, H::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define H_LOG_FMT_INFO(logger, fmt, ...) H_LOG_FMT_LEVEL(logger, H::LogLevel::INFO, fmt, __VA_ARGS__)
#define H_LOG_FMT_WARNING(logger, fmt, ...) H_LOG_FMT_LEVEL(logger, H::LogLevel::WARNING, fmt, __VA_ARGS__)
#define H_LOG_FMT_ERROR(logger, fmt, ...) H_LOG_FMT_LEVEL(logger, H::LogLevel::ERROR, fmt, __VA_ARGS__)
#define H_LOG_FMT_FATAL(logger, fmt, ...) H_LOG_FMT_LEVEL(logger, H::LogLevel::FATAL, fmt, __VA_ARGS__)

/**
 * @brief 获取主日志器
 *
 */
#define H_LOG_ROOT() H::LoggerMgr::GetInstance()->getRoot()

namespace H
{

    class Fiber;
    /**
     * @brief 返回当前线程的id
     *
     * @return pid_t
     */
    pid_t GetThreadId();

    /**
     * @brief 返回当前协程的id
     *
     * @return uint64_t
     */
    uint64_t GetFiberId();
    class FSUtil
    {
    public:
        static void ListAllFile(std::vector<std::string> &files, const std::string &path, const std::string &subfix);
        static bool Mkdir(const std::string &dirname);
        static bool IsRunningPidfile(const std::string &pidfile);
        static bool Rm(const std::string &path);
        static bool Mv(const std::string &from, const std::string &to);
        static bool Realpath(const std::string &path, std::string &rpath);
        static bool Symlink(const std::string &frm, const std::string &to);
        static bool Unlink(const std::string &filename, bool exist = false);
        static std::string Dirname(const std::string &filename);
        static std::string Basename(const std::string &filename);
        static bool OpenForRead(std::ifstream &ifs, const std::string &filename, std::ios_base::openmode mode);
        static bool OpenForWrite(std::ofstream &ofs, const std::string &filename, std::ios_base::openmode mode);
    };
}
