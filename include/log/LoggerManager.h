/*
 * @Author: user8361
 * @Date: 2022-07-07 15:39:03
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-13 17:23:04
 * @FilePath: /projects/HLog/include/LoggerManager.h
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */
#pragma once
#include <map>
#include "utils/Singleton.h"
#include "Logger.h"
namespace H
{
    class Logger;
    class LoggerManager
    {
    public:
        // 自旋锁类型互斥量
        typedef Spinlock MutexType;
        LoggerManager();

        /**
         * @brief 获取日志器
         *
         * @param name 日志器名称
         * @return Logger::m_ptr
         */
        std::shared_ptr<Logger> GetLogger(const std::string &name);
        /**
         * @brief 初始化
         *
         */
        void init();

        /**
         * @brief 返回主日志器
         *
         * @return Logger::m_ptr
         */
        std::shared_ptr<Logger> GetRoot() const { return m_root; }

        /**
         * @brief  将所有的日志器配置 转换成 yaml格式的string
         *
         * @return std::string
         */
        std::string ToYamlString();

    private:
        MutexType m_mutex;                                        // mutex
        std::map<std::string, std::shared_ptr<Logger>> m_loggers; // 日志器容器
        std::shared_ptr<Logger> m_root;                           // 主日志器
    };

    /**
     * @brief 日志管理器的单例指针
     *
     */
    typedef Singleton<LoggerManager> LoggerMgr;
}
