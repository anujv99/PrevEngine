#pragma once

#ifdef PV_PLATFORM_WINDOWS
	#include "platform/windows/misc.h"
#endif

#ifdef PV_PLATFORM_LINUX
	#include "platform/linux/misc.h"
#endif

namespace prev {
	#ifdef PV_PLATFORM_WINDOWS
		#define LOG(...) printf(__VA_ARGS__)
		#ifdef PV_BUILD_LIB
			#define PV_CORE_TRACE(...)		windows::ChangeConsoleColor(windows::CYAN);		LOG("[PREV_ENGINE] "); LOG(__VA_ARGS__); LOG("\n"); windows::ChangeConsoleColor(windows::NORMAL)
			#define PV_CORE_INFO(...)		windows::ChangeConsoleColor(windows::GREEN);	LOG("[PREV_ENGINE] "); LOG(__VA_ARGS__); LOG("\n"); windows::ChangeConsoleColor(windows::NORMAL)
			#define PV_CORE_WARN(...)		windows::ChangeConsoleColor(windows::YELLOW);	LOG("[PREV_ENGINE] "); LOG(__VA_ARGS__); LOG("\n"); windows::ChangeConsoleColor(windows::NORMAL)
			#define PV_CORE_ERROR(...)		windows::ChangeConsoleColor(windows::RED);		LOG("[PREV_ENGINE] "); LOG(__VA_ARGS__); LOG("\n"); windows::ChangeConsoleColor(windows::NORMAL)
			#define PV_CORE_CRITICAL(...)	windows::ChangeConsoleColor(windows::RED_OVER);	LOG("[PREV_ENGINE] "); LOG(__VA_ARGS__); LOG("\n"); windows::ChangeConsoleColor(windows::NORMAL)
		#else
			#define PV_TRACE(...)		windows::ChangeConsoleColor(windows::CYAN);		LOG("[APP] "); LOG(__VA_ARGS__); LOG("\n"); windows::ChangeConsoleColor(windows::NORMAL)
			#define PV_INFO(...)		windows::ChangeConsoleColor(windows::GREEN);	LOG("[APP] "); LOG(__VA_ARGS__); LOG("\n"); windows::ChangeConsoleColor(windows::NORMAL)
			#define PV_WARN(...)		windows::ChangeConsoleColor(windows::YELLOW);	LOG("[APP] "); LOG(__VA_ARGS__); LOG("\n"); windows::ChangeConsoleColor(windows::NORMAL)
			#define PV_ERROR(...)		windows::ChangeConsoleColor(windows::RED);		LOG("[APP] "); LOG(__VA_ARGS__); LOG("\n"); windows::ChangeConsoleColor(windows::NORMAL)
			#define PV_CRITICAL(...)	windows::ChangeConsoleColor(windows::RED_OVER);	LOG("[APP] "); LOG(__VA_ARGS__); LOG("\n"); windows::ChangeConsoleColor(windows::NORMAL)
		#endif
	#endif

	#ifdef PV_PLATFORM_LINUX
		#define LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
		#ifdef PV_BUILD_LIB
			#define PV_CORE_TRACE(fmt, ...)		linux::ChangeConsoleColor(linux::CYAN);		LOG("[PREV_ENGINE] "); LOG(fmt, ##__VA_ARGS__); LOG("\n"); linux::ChangeConsoleColorToNormal()
			#define PV_CORE_INFO(fmt, ...)		linux::ChangeConsoleColor(linux::GREEN);	LOG("[PREV_ENGINE] "); LOG(fmt, ##__VA_ARGS__); LOG("\n"); linux::ChangeConsoleColorToNormal()
			#define PV_CORE_WARN(fmt, ...)		linux::ChangeConsoleColor(linux::YELLOW);	LOG("[PREV_ENGINE] "); LOG(fmt, ##__VA_ARGS__); LOG("\n"); linux::ChangeConsoleColorToNormal()
			#define PV_CORE_ERROR(fmt, ...)		linux::ChangeConsoleColor(linux::RED);		LOG("[PREV_ENGINE] "); LOG(fmt, ##__VA_ARGS__); LOG("\n"); linux::ChangeConsoleColorToNormal()
			#define PV_CORE_CRITICAL(fmt, ...)	linux::ChangeConsoleColor(linux::RED_OVER);	LOG("[PREV_ENGINE] "); LOG(fmt, ##__VA_ARGS__); LOG("\n"); linux::ChangeConsoleColorToNormal()
		#else
			#define PV_TRACE(fmt, ...)		linux::ChangeConsoleColor(linux::CYAN);		LOG("[APP] "); LOG(fmt, ##__VA_ARGS__); LOG("\n"); linux::ChangeConsoleColorToNormal()
			#define PV_INFO(fmt, ...)		linux::ChangeConsoleColor(linux::GREEN);	LOG("[APP] "); LOG(fmt, ##__VA_ARGS__); LOG("\n"); linux::ChangeConsoleColorToNormal()
			#define PV_WARN(fmt, ...)		linux::ChangeConsoleColor(linux::YELLOW);	LOG("[APP] "); LOG(fmt, ##__VA_ARGS__); LOG("\n"); linux::ChangeConsoleColorToNormal()
			#define PV_ERROR(fmt, ...)		linux::ChangeConsoleColor(linux::RED);		LOG("[APP] "); LOG(fmt, ##__VA_ARGS__); LOG("\n"); linux::ChangeConsoleColorToNormal()
			#define PV_CRITICAL(fmt, ...)	linux::ChangeConsoleColor(linux::RED_OVER);	LOG("[APP] "); LOG(fmt, ##__VA_ARGS__); LOG("\n"); linux::ChangeConsoleColorToNormal()
		#endif
	#endif
}