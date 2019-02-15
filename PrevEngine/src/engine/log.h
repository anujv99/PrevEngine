#pragma once

#ifdef PV_PLATFORM_WINDOWS
	#include "platform/windows/misc.h"
#endif

#ifdef PV_PLATFORM_LINUX
	#include "platform/linux/misc.h"
#endif

namespace prev {

	enum class LogLevel {
		PV_TRACE,
		PV_INFO,
		PV_WARN,
		PV_ERROR,
		PV_CRITICAL
	};

	typedef std::function<void(std::string, LogLevel level) > LogCallbackFunc;

	class Log {
	public:
		static void SetLoggerCallbackFunction(LogCallbackFunc func);
		static void CallCallback(LogLevel level, std::string &message); // Do not call this function anywhere
		static void ShouldShowLogOnTerminal(bool shouldShow);
		inline static bool IsLogShowingOnTerminal() { return shouldShowLogOnTerminal; }
	private:
		struct Log_Message {
			std::string message;
			LogLevel level;
		};
		static std::vector<LogCallbackFunc> m_CallbackFunctions;
		static std::vector<Log_Message> m_Buffer;
		static bool shouldShowLogOnTerminal;
	};

	#ifdef PV_PLATFORM_WINDOWS
		#define MAX_LOG 2048
		#define CORE_LOG_CALLBACK(level, ...) { char logMessage[MAX_LOG]; sprintf(logMessage, __VA_ARGS__); Log::CallCallback(level, "[PREV_ENGINE] " + std::string(logMessage)); }
		#define LOG_CALLBACK(level, ...) { char logMessage[MAX_LOG]; sprintf(logMessage, __VA_ARGS__); prev::Log::CallCallback(level, "[APP] " + std::string(logMessage)); }
	
		#define LOG(...) if(prev::Log::IsLogShowingOnTerminal()) printf(__VA_ARGS__)
		#if defined PV_LOG_DISABLED
			#define PV_TRACE(...)
			#define PV_INFO(...)
			#define PV_WARN(...)
			#define PV_ERROR(...)
			#define PV_CRITICAL(...)
			#define PV_CORE_TRACE(...)
			#define PV_CORE_INFO(...)
			#define PV_CORE_WARN(...)
			#define PV_CORE_ERROR(...)
			#define PV_CORE_CRITICAL(...)
		#elif defined PV_BUILD_LIB
			#define PV_CORE_TRACE(...)		windows::ChangeConsoleColor(windows::CYAN);		LOG("[PREV_ENGINE] "); LOG(__VA_ARGS__); LOG("\n"); CORE_LOG_CALLBACK(LogLevel::PV_TRACE, __VA_ARGS__)
			#define PV_CORE_INFO(...)		windows::ChangeConsoleColor(windows::GREEN);	LOG("[PREV_ENGINE] "); LOG(__VA_ARGS__); LOG("\n"); CORE_LOG_CALLBACK(LogLevel::PV_INFO, __VA_ARGS__)
			#define PV_CORE_WARN(...)		windows::ChangeConsoleColor(windows::YELLOW);	LOG("[PREV_ENGINE] "); LOG(__VA_ARGS__); LOG("\n"); CORE_LOG_CALLBACK(LogLevel::PV_WARN, __VA_ARGS__)
			#define PV_CORE_ERROR(...)		windows::ChangeConsoleColor(windows::RED);		LOG("[PREV_ENGINE] "); LOG(__VA_ARGS__); LOG("\n"); CORE_LOG_CALLBACK(LogLevel::PV_ERROR, __VA_ARGS__)
			#define PV_CORE_CRITICAL(...)	windows::ChangeConsoleColor(windows::RED_OVER);	LOG("[PREV_ENGINE] "); LOG(__VA_ARGS__); LOG("\n"); CORE_LOG_CALLBACK(LogLevel::PV_CRITICAL, __VA_ARGS__)
		#else
			#define PV_TRACE(...)		prev::windows::ChangeConsoleColor(prev::windows::CYAN);\
										LOG("[APP] "); LOG(__VA_ARGS__); LOG("\n"); LOG_CALLBACK(prev::LogLevel::PV_TRACE, __VA_ARGS__)

			#define PV_INFO(...)		prev::windows::ChangeConsoleColor(prev::windows::GREEN);\
										LOG("[APP] "); LOG(__VA_ARGS__); LOG("\n"); LOG_CALLBACK(prev::LogLevel::PV_INFO, __VA_ARGS__)

			#define PV_WARN(...)		prev::windows::ChangeConsoleColor(prev::windows::YELLOW);\
										LOG("[APP] "); LOG(__VA_ARGS__); LOG("\n"); LOG_CALLBACK(prev::LogLevel::PV_WARN, __VA_ARGS__)

			#define PV_ERROR(...)		prev::windows::ChangeConsoleColor(prev::windows::RED);\
										LOG("[APP] "); LOG(__VA_ARGS__); LOG("\n"); LOG_CALLBACK(prev::LogLevel::PV_ERROR, __VA_ARGS__)

			#define PV_CRITICAL(...)	prev::windows::ChangeConsoleColor(prev::windows::RED_OVER);\
										LOG("[APP] "); LOG(__VA_ARGS__); LOG("\n"); LOG_CALLBACK(prev::LogLevel::PV_CRITICAL, __VA_ARGS__)

		#endif
	#endif

	#ifdef PV_PLATFORM_LINUX
		#define LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
		#ifdef PV_BUILD_LIB
			#define PV_CORE_TRACE(fmt, ...)		linux::ChangeConsoleColor(linux::CYAN);		printf("[PREV_ENGINE] "); LOG(fmt, ##__VA_ARGS__); printf("\n")
			#define PV_CORE_INFO(fmt, ...)		linux::ChangeConsoleColor(linux::GREEN);	printf("[PREV_ENGINE] "); LOG(fmt, ##__VA_ARGS__); printf("\n")
			#define PV_CORE_WARN(fmt, ...)		linux::ChangeConsoleColor(linux::YELLOW);	printf("[PREV_ENGINE] "); LOG(fmt, ##__VA_ARGS__); printf("\n")
			#define PV_CORE_ERROR(fmt, ...)		linux::ChangeConsoleColor(linux::RED);		printf("[PREV_ENGINE] "); LOG(fmt, ##__VA_ARGS__); printf("\n")
			#define PV_CORE_CRITICAL(fmt, ...)	linux::ChangeConsoleColor(linux::RED_OVER);	printf("[PREV_ENGINE] "); LOG(fmt, ##__VA_ARGS__); printf("\n")
		#else
			#define PV_TRACE(fmt, ...)		linux::ChangeConsoleColor(linux::CYAN);		printf("[APP] "); LOG(fmt, ##__VA_ARGS__); printf("\n")
			#define PV_INFO(fmt, ...)		linux::ChangeConsoleColor(linux::GREEN);	printf("[APP] "); LOG(fmt, ##__VA_ARGS__); printf("\n")
			#define PV_WARN(fmt, ...)		linux::ChangeConsoleColor(linux::YELLOW);	printf("[APP] "); LOG(fmt, ##__VA_ARGS__); printf("\n")
			#define PV_ERROR(fmt, ...)		linux::ChangeConsoleColor(linux::RED);		printf("[APP] "); LOG(fmt, ##__VA_ARGS__); printf("\n")
			#define PV_CRITICAL(fmt, ...)	linux::ChangeConsoleColor(linux::RED_OVER);	printf("[APP] "); LOG(fmt, ##__VA_ARGS__); printf("\n")
		#endif
	#endif
}