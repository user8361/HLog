/*
 * @Author: user8361
 * @Date: 2022-07-08 10:26:04
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-13 17:16:48
 * @FilePath: /projects/HLog/src/LoggerManager.cpp
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#include "LoggerManager.h"
namespace H
{
    LoggerManager::LoggerManager()
    {

        m_root.reset(new Logger);
        m_root->addApender(LogAppender::m_ptr(new StdoutLogAppender));
        m_loggers[m_root->m_name] = m_root;

        init();
    }
    void LoggerManager::init()
    {
    }

    std::shared_ptr<Logger> LoggerManager::GetLogger(const std::string &name)
    {
        MutexType::m_lock lock(m_mutex);
        auto it = m_loggers.find(name);

        if (it != m_loggers.end())
        {
            return it->second;
        }
        std::shared_ptr<Logger> logger(new Logger);
        logger->m_name = name;
        return logger;
    }

    std::string LoggerManager::ToYamlString()
    {
        MutexType::m_lock lock(m_mutex);
        YAML::Node node;
        for (auto &i : m_loggers)
        {
            node.push_back(YAML::Load(i.second->toYamlString()));
        }
        std::stringstream ss;
        ss << node;

        return ss.str();
    }
}