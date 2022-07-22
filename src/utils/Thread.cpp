/*
 * @Author: user8361
 * @Date: 2022-07-13 13:52:43
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-17 22:08:02
 * @FilePath: /projects/HLog/src/utils/Thread.cpp
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */
#include "utils/Thread.h"
#include "log/Logger.h"
#include "utils/Utils.h"
namespace H
{
    static thread_local Thread *t_thread = nullptr;
    static thread_local std::string t_thread_name = "UNKNOW";
    static H::Logger::m_ptr g_logger = H_LOG_NAME("system");

    Thread::Thread(std::function<void()> cb, const std::string &name) : m_cb(cb), m_name(name)
    {
        if (name.empty())
        {
            m_name = "UNKNOW";
        }
        int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
        if (rt)
        {
            H_LOG_ERROR(g_logger) << "pthread_create thread fail, rt=" << rt
                                  << " name=" << name;
            throw std::logic_error("pthread_create error");
        }
        m_semphore.wait();
    }
    Thread::~Thread()
    {
        if (m_thread)
        {
            pthread_detach(m_thread);
        }
    }
    void Thread::join()
    {
        if (m_thread)
        {
            int ret = pthread_join(m_thread, nullptr);
            if (ret)
            {
                H_LOG_ERROR(g_logger) << "pthread_join thread fail, ret=" << ret << ", name=" << m_name;
            }
            m_thread = 0;
        }
    }

    void *Thread::run(void *arg)
    {
        Thread *thread = (Thread *)arg;
        t_thread = thread;
        t_thread_name = thread->m_name;
        thread->m_pid = H::GetThreadId();
        pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());
        std::function<void()> cb;
        cb.swap(thread->m_cb);

        thread->m_semphore.notify();
        cb();
        return 0;
    }

    Thread *Thread::GetThis()
    {
        return t_thread;
    }
    const std::string &Thread::GetName()
    {
        return t_thread_name;
    }

    void Thread::SetName(const std::string &name)
    {
        if (name.empty())
        {
            return;
        }

        if (t_thread)
        {
            t_thread->m_name = name;
        }

        t_thread_name = name;
    }
}