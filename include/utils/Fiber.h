/*
 * @Author: user8361
 * @Date: 2022-07-09 23:26:58
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-17 22:14:18
 * @FilePath: /projects/HLog/include/utils/Fiber.h
 * @Description:  封装协程
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */
#pragma once

#include <functional>
#include <memory>
#include <ucontext.h>
// #include "Utils.h"
namespace H
{
    class Fiber : public std::enable_shared_from_this<Fiber>
    {
    private:
        friend class Schedurler;

    public:
        typedef std::shared_ptr<Fiber> m_ptr;

        enum State
        {
            INIT,  // 初始态
            HOLD,  // 阻塞态
            EXEC,  // 执行态
            TERM,  // 终止态
            READY, // 就绪态
            EXCEPT // 异常
        };

    private:
        /**
         * @brief 无参构造函数
         *
         * 每个线程第一个协程的构造
         */
        Fiber();

    public:
        /**
         * @brief 构造函数
         *
         * @param cb 协程执行的函数
         * @param stacksize 协程栈大小
         * @param use_caller 是否在main fiber 上调度
         */
        Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller = false);
        ~Fiber();

        /**
         * @brief 重置协程执行函数，并设置状态
         * @pre getState() --> INIT , TERM , EXCEPT
         * @post getState() == INIT
         * @param cb
         */
        void reser(std::function<void()> cb);

        /**
         * @brief 将当前协程切换到运行态
         * @pre getState() != EXEC
         * @post getState() == EXEC
         */
        void swapIn();
        /**
         * @brief 将当前协程切换到后台
         *
         */
        void swapOut();

        /**
         * @brief 将当前线程切换到执行状态
         * @pre 执行的为当前线程的主协程
         *
         */
        void call();

        /**
         * @brief 将当前线程切换到后台
         * @pre 执行的为该协程
         * @post 返回到线程的主协程
         */
        void back();

        /**
         * @brief 返回协程id
         *
         * @return uint32_t
         */
        uint64_t getId() const { return m_id; }

        /**
         * @brief 返回协程状态
         *
         * @return State
         */
        State getState() const { return m_state; }

    public:
        /**
         * @brief 设置当前线程的运行协程
         *
         * @param f 运行协程
         */
        static void SetThis(Fiber *f);

        /**
         * @brief 返回当前所在的协程
         *
         * @return Fiber::m_ptr
         */
        static Fiber::m_ptr GetThis();
        /**
         * @brief 将当前协程切换到后台，并设置为ready状态
         * @post getState() == READY
         */
        static void YieldToReady();
        /**
         * @brief 将当前协程切换到后台，并设置为HOLD状态
         * @post getState() == HOLD
         */
        static void YieldToHold();
        /**
         * @brief 返回当前协程的总数量
         *
         * @return uint64_t
         */
        static uint64_t TotalFibers();
        /**
         * @brief 协程执行函数
         * @post 执行完毕后返回到线程主协程
         */
        static void MainFunc();
        /**
         * @brief 协程执行函数
         * @post 执行完成返回到线程调度协程
         */
        static void CallerMainFunc();

        /**
         * @brief 返回当前协程id
         *
         * @return uint64_t
         */
        static uint64_t GetFiberId();

    private:
        uint64_t m_id = 0;          // 协程id
        uint32_t m_stacksize = 0;   // 协程运行栈大小
        State m_state = INIT;       // 协程状态
        ucontext_t m_content;       // 协程上下文
        void *m_stack = nullptr;    // 协程运行栈指针
        std::function<void()> m_cb; // 协程运行函数
    };

}
