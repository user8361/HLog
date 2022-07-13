/*
 * @Author: user8361
 * @Date: 2022-07-10 18:27:01
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-10 19:33:11
 * @FilePath: /projects/HLog/include/Singleton.h
 * @Description: 单例模式
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#pragma once
#include <memory>

namespace H
{
    namespace
    {
        template <class T, class X, int N>
        T &GetInstanceX()
        {
            static T v;
            return &v;
        }

        template <class T, class X, int N>
        std::shared_ptr<T> GetInstancePtr()
        {
            static std::shared_ptr<T> v(new T);
            return v;
        }

    }

    /**
     * @brief 单例模式封装类
     *
     * @tparam T 类型
     * @tparam X 为了创造多个实例对应的tag
     * @tparam N 同一个tag创造多个实例索引
     */
    template <class T, class X = void, int N = 0>
    class Singleton
    {
    public:
        /**
         * @brief 返回单例裸指针
         *
         * @return T*
         */
        static T *GetInstance()
        {
            static T v;
            return &v;
        }
    };

    /**
     * @brief 单例模式智能指针封装
     *
     * @tparam T 类型
     * @tparam X 为了创造多个实例对应的 TAG
     * @tparam N 同一个TAG创造多个实例索引
     */
    template <class T, class X, int N = 0>
    class SingletonPtr
    {
    public:
        /**
         * @brief 返回单例智能指针
         *
         * @return std::shared_ptr<T>
         */
        static std::shared_ptr<T> GetInstancePtr()
        {
            static std::shared_ptr<T> v(new T);
            return v;
        }
    };

} // namespace H
