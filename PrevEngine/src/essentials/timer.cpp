#include "pch.h"
#include "timer.h"

namespace prev {

	std::chrono::duration<float> Timer::m_DeltaTime;
	pv_time_point Timer::m_Time = std::chrono::high_resolution_clock::now();
	pv_time_point Timer::m_StartTime = std::chrono::high_resolution_clock::now();

	unsigned int Timer::m_FPS						= 0;
	unsigned long long int Timer::m_LastTimeSec		= 0;
	bool Timer::shouldShowFPS						= false;

	void Timer::Update() {
		auto currentTime = std::chrono::high_resolution_clock::now();
		m_DeltaTime = currentTime - m_Time;
		m_Time = currentTime;
		m_FPS++;
		if ((unsigned long long int)GetTime() > m_LastTimeSec) {
			m_LastTimeSec++;
			if (shouldShowFPS) {
				PV_CORE_TRACE("[FPS = %d]", m_FPS);
			}
			m_FPS = 0;
		}
		
	}

	float Timer::GetTime() {
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_Time - m_StartTime);
		return (float)duration.count() / 1000000;
	}

	float Timer::GetDeltaTime() {
		return m_DeltaTime.count();
	}

	void Timer::FPSCounter(bool isVisible) {
		shouldShowFPS = isVisible;
	}

	TimeThis::TimeThis(bool timeInMs) {
		m_Start = std::chrono::high_resolution_clock::now();
		isMS = timeInMs;
	}

	TimeThis::~TimeThis() {
		std::chrono::duration<float> deltaTime = std::chrono::high_resolution_clock::now() - m_Start;
		if (isMS) {
			PV_CORE_WARN("This Scope Took : %.6fms", deltaTime.count() * 1000);
		} else {
			PV_CORE_WARN("This Scope Took : %fs", deltaTime.count());
		}
	}

}
