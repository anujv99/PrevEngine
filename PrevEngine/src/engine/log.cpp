#include "pch.h"
#include "log.h"

namespace prev {

	std::vector<LogCallbackFunc> Log::m_CallbackFunctions;
	std::vector<Log::Log_Message> Log::m_Buffer;
	bool Log::shouldShowLogOnTerminal = true;

	void Log::SetLoggerCallbackFunction(LogCallbackFunc func) {
		m_CallbackFunctions.push_back(func);
	}

	void Log::CallCallback(LogLevel level, std::string &message) {
		m_Buffer.push_back({message + "\n", level});
		for (auto fn : m_CallbackFunctions) {
			for (auto buf : m_Buffer) {
				fn(buf.message, buf.level);
			}
			m_Buffer.clear();
		}
	}

	void Log::ShouldShowLogOnTerminal(bool shouldShow) {
		shouldShowLogOnTerminal = shouldShow;
	}

}