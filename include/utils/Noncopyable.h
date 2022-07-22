/*
 * @Author: user8361
 * @Date: 2022-07-07 19:10:23
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-12 14:50:09
 * @FilePath: /projects/HLog/include/Noncopyable.h
 * @Description: 禁用拷贝构造函数和赋值操作符，构造函数和析构函数使用默认的
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#pragma once

namespace H
{
    class Noncopyable
    {
    public:
        Noncopyable() = default;                              // 默认构造函数
        ~Noncopyable() = default;                             // 默认析构函数
        Noncopyable(const Noncopyable &) = delete;            // 禁用拷贝构造函数
        Noncopyable &operator=(const Noncopyable &) = delete; // 禁用赋值操作符
    };
}
