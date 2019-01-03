#pragma once

#ifdef PV_PLATFORM_WINDOWS
	#include "platform/windows/misc.h"
#endif

namespace prev {
	#define LOG(...) printf(__VA_ARGS__)

	#ifdef PV_PLATFORM_WINDOWS
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
	#else
		#define PV_CORE_TRACE(...)		LOG(__VA_ARGS__)
		#define PV_CORE_INFO(...)		LOG(__VA_ARGS__)
		#define PV_CORE_WARN(...)		LOG(__VA_ARGS__)
		#define PV_CORE_ERROR(...)		LOG(__VA_ARGS__)
		#define PV_CORE_CRITICAL(...)	LOG(__VA_ARGS__)
	#endif
}