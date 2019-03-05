#pragma once

#ifdef PV_DEBUG
#endif

#ifdef PV_ENABLE_ASSERTS
	#ifdef PV_PLATFORM_WINDOWS
		#define PV_ASSERT(x, ...) { if (!(x)) { PV_CORE_ERROR("Assertion Failed: ");\
									PV_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
	#else
		#define PV_ASSERT(x, fmt, ...) { if (!(x)) { PV_CORE_ERROR("Assertion Failed: ");\
									PV_CORE_ERROR(fmt, ##__VA_ARGS__); } }
	#endif
#else
	#define PV_ASSERT(x, ...)
#endif
