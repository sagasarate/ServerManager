// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料


#define USE_CRT_DETAIL_NEW
#include <afx.h>
#endif

#include "../../Libs/Utils/Utils.h"
// TODO: 在此处引用程序需要的其他头文件

extern UINT g_LogChannel;


inline void LogWithTag(LPCTSTR szTag, LPCTSTR Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(g_LogChannel, ILogPrinter::LOG_LEVEL_NORMAL, szTag, Format, vl);
	va_end(vl);
}

inline void LogDebugWithTag(LPCTSTR szTag, LPCTSTR Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(g_LogChannel, ILogPrinter::LOG_LEVEL_DEBUG, szTag, Format, vl);
	va_end(vl);
}

#define Log(_Format, ...)	LogWithTag(_T(__FUNCTION__), _Format, ##__VA_ARGS__)
#define LogDebug(_Format, ...)	LogDebugWithTag(_T(__FUNCTION__), _Format, ##__VA_ARGS__)

#include "Protocol.h"