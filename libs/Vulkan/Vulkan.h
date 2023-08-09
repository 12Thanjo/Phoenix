#pragma once



#if defined(PH_PLATFORM_WINDOWS)
	#define VK_USE_PLATFORM_WIN32_KHR
	#define NOMINMAX
#else
	#error Unsupported platform
#endif

#include <vulkan/vulkan.h>

