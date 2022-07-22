/*
 * @Author: user8361
 * @Date: 2022-07-07 19:00:27
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-13 16:12:43
 * @FilePath: /projects/HLog/include/Mutex.h
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */
#pragma once
#include "Noncopyable.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdexcept>
namespace H
{

    /**
     * @brief 模板实现 局部锁
     *
     * @tparam T
     */
    template <class T>
    class ScopedLockImpl
    {
    public:
        /**
         * @brief Construct a new Scoped Lock Impl object
         *
         * @param mutex
         */
        ScopedLockImpl(T &mutex) : m_mutex(mutex)
        {
            m_mutex.lock();
            m_locked = true;
        }
        /**
         * @brief Destroy the Scoped Lock Impl object
         *
         */
        ~ScopedLockImpl()
        {
            unlock();
        }

        /**
         * @brief 加锁
         *
         */
        void lock()
        {
            if (!m_locked)
            {
                m_mutex.lock();
                m_locked = true;
            }
        }

        /**
         * @brief 解锁
         *
         */
        void unlock()
        {
            if (m_locked)
            {
                m_mutex.unlock();
                m_locked = false;
            }
        }

    private:
        T &m_mutex;    // mutex
        bool m_locked; // 是否已经上锁
    };

    /**
     * @brief 互斥量
     *
     */
    class Mutex : Noncopyable
    {
    public:
        // 局部锁
        typedef ScopedLockImpl<Mutex> m_lock;

        /**
         * @brief Construct a new Mutex object
         *
         */
        Mutex()
        {
            pthread_mutex_init(&m_mutex, nullptr);
        }

        /**
         * @brief Destroy the Mutex object
         *
         */
        ~Mutex()
        {
            pthread_mutex_destroy(&m_mutex);
        }

        /**
         * @brief 加锁
         *
         */
        void lock()
        {
            pthread_mutex_lock(&m_mutex);
        }

        /**
         * @brief 解锁
         *
         */
        void unlock()
        {
            pthread_mutex_unlock(&m_mutex);
        }

    private:
        // mutex
        pthread_mutex_t m_mutex;
    };

    /**
     * @brief 自旋锁
     *
     */
    class Spinlock : Noncopyable
    {
    public:
        // 局部锁
        typedef ScopedLockImpl<Spinlock> m_lock;

        /**
         * @brief Construct a new Spinlock object
         *
         */
        Spinlock()
        {
            // pthread_spin_init0
            pthread_spin_init(&m_mutex, 0);
        }

        /**
         * @brief Destroy the Spinlock object
         *
         */
        ~Spinlock()
        {
            pthread_spin_destroy(&m_mutex);
        }

        /**
         * @brief 枷锁
         *
         */
        void lock()
        {
            pthread_spin_lock(&m_mutex);
        }

        /**
         * @brief 解锁
         *
         */
        void unlock()
        {
            pthread_spin_unlock(&m_mutex);
        }

    private:
        pthread_spinlock_t m_mutex;
    };

    class Semaphore : public Noncopyable
    {
    public:
        /**
         * @brief Construct a new Semaphore object
         *
         * @param count  信号量值的大小
         */
        Semaphore(uint32_t count = 0)
        {
            if (sem_init(&m_semaphore, 0, count))
            {
                throw std::logic_error("sem_init error");
            }
        }

        ~Semaphore()
        {
            sem_destroy(&m_semaphore);
        }

        void wait()
        {
            if (sem_wait(&m_semaphore))
            {
                throw std::logic_error("sem_wait error");
            }
        }

        void notify()
        {
            if (sem_post(&m_semaphore))
            {
                throw std::logic_error("sem_post error");
            }
        }

    private:
        sem_t m_semaphore;
    };

}
