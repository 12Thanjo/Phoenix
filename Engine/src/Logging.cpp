#include "Logging.h"

#include <format>

#if defined(PH_PLATFORM_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#define NOCOMM
	#define NOMINMAX
	#include <windows.h>
#endif


#include <libs/Evo/Evo.h>


namespace ph{

	enum class LogLevel{
		Fatal,
		Error,
		Warning,
		Info,
		Debug,
		Trace,
	};


	static auto log_impl(const std::string& msg, LogLevel level) noexcept -> void {
		std::string log_title;

		switch(level){
			case LogLevel::Fatal: {
				log_title = "[FATAL]";
				evo::styleConsoleFatal();
			} break;

			case LogLevel::Error: {
				log_title = "[ERROR]";
				evo::styleConsoleError();
			} break;

			case LogLevel::Warning: {
				log_title = "[WARN] ";
				evo::styleConsoleWarning();
			} break;

			case LogLevel::Info: {
				log_title = "[INFO] ";
				evo::styleConsoleInfo();
			} break;

			case LogLevel::Debug: {
				log_title = "[DEBUG]";
				evo::styleConsoleDebug();
			} break;

			case LogLevel::Trace: {
				log_title = "[TRACE]";
				evo::styleConsoleTrace();
			} break;
		};


		const auto formatted_message = std::format("{} {}\n", log_title, msg);

		evo::log(formatted_message);

		evo::styleConsoleReset();
	};



	
	auto Logging::fatal(const std::string& msg) noexcept -> void {
		log_impl(msg, LogLevel::Fatal);
	};

	auto Logging::error(const std::string& msg) noexcept -> void {
		log_impl(msg, LogLevel::Error);
	};

	auto Logging::warning(const std::string& msg) noexcept -> void {
		log_impl(msg, LogLevel::Warning);
	};

	auto Logging::info(const std::string& msg) noexcept -> void {
		log_impl(msg, LogLevel::Info);
	};

	auto Logging::debug(const std::string& msg) noexcept -> void {
		log_impl(msg, LogLevel::Debug);
	};

	auto Logging::trace(const std::string& msg) noexcept -> void {
		log_impl(msg, LogLevel::Trace);
	};

};
