#pragma once

#define TIME_THIS_SCOPE		prev::TimeThis timer;
#define TIME_THIS_SCOPE_MS	prev::TimeThis timer(true);

namespace prev {

	class Timer {
	public:
		static void Update();
		static float GetTime();
		static float GetDeltaTime();
		static void FPSCounter(bool isVisible);
	private:
		static std::chrono::duration<float> m_DeltaTime;
		static std::chrono::time_point<std::chrono::steady_clock> m_Time, m_StartTime;
		static unsigned int m_FPS;
		static unsigned long long int m_LastTimeSec;
		static bool shouldShowFPS;
	};

	//Time any scope
	class TimeThis {
	public:
		TimeThis(bool timeInMs = false);
		~TimeThis();
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
		bool isMS;
	};

}
