#pragma once

#define TIME_THIS_SCOPE		prev::TimeThis timer;
#define TIME_THIS_SCOPE_MS	prev::TimeThis timer(true);

#if defined _MSC_VER
typedef std::chrono::time_point<std::chrono::steady_clock> pv_time_point;
#elif defined PV_PLATFORM_LINUX
typedef std::chrono::time_point<std::chrono::system_clock> pv_time_point;
#endif

namespace prev {

	class Timer {
	public:
		static void Update();
		static float GetTime();
		static float GetDeltaTime();
		static void FPSCounter(bool isVisible);
		inline static bool IsLoggingFPSCounter() {return shouldShowFPS; }
	private:
		static std::chrono::duration<float> m_DeltaTime;
		static pv_time_point m_Time, m_StartTime;
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
		pv_time_point m_Start;
		bool isMS;
	};

}
