#pragma once

#include "Phoenix/defines.h"

#include <string>

namespace ph{
	
	namespace Logging{
		
		auto fatal(const std::string& msg) noexcept -> void;
		auto error(const std::string& msg) noexcept -> void;
		auto warning(const std::string& msg) noexcept -> void;
		auto info(const std::string& msg) noexcept -> void;
		auto debug(const std::string& msg) noexcept -> void;
		auto trace(const std::string& msg) noexcept -> void;

	};


	#define PH_FATAL(msg) ::ph::Logging::fatal(msg)
	#define PH_ERROR(msg) ::ph::Logging::error(msg)
	#define PH_WARNING(msg) ::ph::Logging::warning(msg)
	#define PH_INFO(msg) ::ph::Logging::info(msg)

	#if defined(PH_CONFIG_DEBUG)
		#define PH_DEBUG(msg) ::ph::Logging::debug(msg)
	#else
		#define PH_DEBUG(msg)
	#endif

	#if defined(PH_CONFIG_TRACE)
		#define PH_TRACE(msg) ::ph::Logging::trace(msg)
	#else
		#define PH_TRACE(msg)
	#endif



	#if defined(PH_CONFIG_DEBUG)
		#if defined(PH_COMPILER_MSVC)
			#define PH_FATAL_BREAK(msg) PH_FATAL(msg); PH_DEBUG_BREAK(); __assume(false);
		#else
			#define PH_FATAL_BREAK(msg) PH_FATAL(msg); PH_DEBUG_BREAK(); __builtin_unreachable();
		#endif


		#define PH_ASSERT(cond, message) { \
				if((cond) == false){ \
					PH_FATAL(message); \
					PH_DEBUG_BREAK(); \
				} \
			}
	#else
		#define PH_FATAL_BREAK(msg) PH_FATAL(msg); std::exit(EXIT_FAILURE);

		#define PH_ASSERT(cond, message)
	#endif
};