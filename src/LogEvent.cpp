/*
 * @Author: user8361
 * @Date: 2022-07-12 14:41:59
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-12 14:47:42
 * @FilePath: /projects/HLog/src/LogEvent.cpp
 * @Description:
 *
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved.
 */

#include "LogEvent.h"
#include "Logger.h"

namespace H
{
    LogEventWrap ::~LogEventWrap()
    {
        m_event->GetLogger()->log(m_event->GetLevel(), m_event);
    }
}
