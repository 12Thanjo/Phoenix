#pragma once



//////////////////////////////////////////////////////////////////////
// compilers

#if defined(_MSC_VER)
	#define PH_COMPILER_MSVC
#elif defined(__clang__)
	#define PH_COMPILER_CLANG
#elif defined(__gcc__)
	#define PH_COMPILER_GCC
#else
	#error Unknown or unsupported compiler
#endif



//////////////////////////////////////////////////////////////////////
// DLL import / export

#if defined(PH_EXPORT)
	// dll export

	#if defined(PH_COMPILER_MSVC)
		#define PH_API __declspec(dllexport)
	#else
		#define PH_API __attribute__((visibility("default")))
	#endif

#else
	// dll import

	#if defined(PH_COMPILER_MSVC)
		#define PH_API __declspec(dllimport)
	#else 
		#define PH_API 
	#endif

#endif




//////////////////////////////////////////////////////////////////////
// misc


// putting this in front of a function messes up the Sublime's syntax highlighting
#define PH_NODISCARD [[nodiscard]]



#if defined(PH_CONFIG_DEBUG)
	#if defined(PH_COMPILER_MSVC)
		#include <intrin.h>
		#define PH_DEBUG_BREAK() __debugbreak()
	#else
		#define PH_DEBUG_BREAK() __builtin_trap()
	#endif

#else
	#define PH_DEBUG_BREAK() static_assert(false, "PH_DEBUG_BREAK() should not be called in release mode")
#endif
