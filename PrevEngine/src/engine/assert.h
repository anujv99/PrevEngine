#pragma once

#ifdef PV_ENABLE_ASSERTS
	#ifdef PV_PLATFORM_WINDOWS
		#define PV_ASSERT(x, ...) { if (!(x)) { PV_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
									PV_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
	#endif
#else
	#define PV_ASSERT(x, ...)
#endif
