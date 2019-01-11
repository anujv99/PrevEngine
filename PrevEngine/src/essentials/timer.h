#pragma once

#define TIME_THIS_SCOPE prev::TimeThis timer;

namespace prev {

	class Timer {
	public:
		static void Update();
		static float GetTime();
		static float GetDeltaTime();
	private:
		static std::chrono::duration<float> m_DeltaTime;
		static std::chrono::time_point<std::chrono::steady_clock> m_Time, m_StartTime;
	};

	//Time any scope
	class TimeThis {
	public:
		TimeThis();
		~TimeThis();
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
	};

}
