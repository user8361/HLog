/*
 * @Author: user8361
 * @Date: 2022-07-13 13:29:56
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-13 16:06:48
 * @FilePath: /projects/HLog/include/Thread.h
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */
#pragma once
#include "Mutex.h"
#include <memory>
#include <functional>
namespace H
{
    class Thread : public Noncopyable
    {
    public:
        typedef std::shared_ptr<Thread> m_ptr;
        Thread(std::function<void()> cb, const std::string &name);
        ~Thread();

        /**
         * @brief  返回线程id
         *
         * @return pid_t
         */
        pid_t getPid() const { return m_pid; }

        /**
         * @brief 返回线程名称
         *
         * @return std::string
         */
        std::string getName() const { return m_name; }

        /**
         * @brief 等待线程执行完成
         *
         */
        void join();

        /**
         * @brief 返回当前线程指针
         *
         * @return Thread*
         */
        static Thread *GetThis();

        /**
         * @brief 返回当前工作的线程名称
         *
         * @return const std::string&
         */
        static const std::string &GetName();
        /**
         * @brief 设置当前线程的名称
         *
         * @param name
         */
        static void SetName(const std::string &name);

    private:
        /**
         * @brief 线程执行函数
         *
         * @param arg
         * @return void*
         */
        static void *run(void *arg);

    private:
        pid_t m_pid = -1;           // 线程id
        pthread_t m_thread = 0;     // 线程结构
        std::function<void()> m_cb; // 线程执行函数
        std::string m_name;         // 线程名称
        Semaphore m_semphore;       // 信号量
    };
}
