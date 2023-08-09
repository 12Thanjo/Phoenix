#pragma once



#if defined(PH_PLATFORM_WINDOWS)
	#define VK_USE_PLATFORM_WIN32_KHR

	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN
	#define NOCOMM
	#define NOMINMAX
#else
	#error Unsupported platform
#endif



#define GLFW_INCLUDE_NONE // no OpenGL
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#if defined(PH_PLATFORM_WINDOWS)
	#define GLFW_EXPOSE_NATIVE_WIN32
#else
	#error Unsupported platform
#endif
#include <GLFW/glfw3native.h>