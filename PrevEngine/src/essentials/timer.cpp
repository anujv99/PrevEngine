#include "pch.h"
#include "timer.h"

namespace prev {

	std::chrono::duration<float> Timer::m_DeltaTime;
	std::chrono::time_point<std::chrono::steady_clock> Timer::m_Time = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> Timer::m_StartTime = std::chrono::high_resolution_clock::now();

	void Timer::Update() {
		auto currentTime = std::chrono::high_resolution_clock::now();
		m_DeltaTime = currentTime - m_Time;
		m_Time = currentTime;
	}

	float Timer::GetTime() {
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_Time - m_StartTime);
		return (float)duration.count() / 1000000;
	}

	float Timer::GetDeltaTime() {
		return m_DeltaTime.count();
	}

	TimeThis::TimeThis() {
		m_Start = std::chrono::high_resolution_clock::now();
	}

	TimeThis::~TimeThis() {
		std::chrono::duration<float> deltaTime = std::chrono::high_resolution_clock::now() - m_Start;
		std::cout << deltaTime.count() << std::endl;
	}

}
