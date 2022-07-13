/*
 * @Author: user8361
 * @Date: 2022-07-10 09:05:47
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-11 15:33:11
 * @FilePath: /projects/HLog/src/Fiber.cpp
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#include "Fiber.h"
#include "Logger.h"

namespace H
{
    // static Logger::m_ptr g_logger = H_LOG_NAME("system");
    static std::atomic<uint64_t> s_fiber_id{0}; // atomic 自增
    static std::atomic<uint64_t> s_fiber_count{0};
    static thread_local Fiber *t_fiber = nullptr;

    static thread_local Fiber::m_ptr g_threadFiber = nullptr;

    // static thread_local Fiber::Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller = false) : m_id(), m_cb(cb)
    // {
    // }

    uint64_t Fiber::GetFiberId()
    {
        return 0;
    }
}