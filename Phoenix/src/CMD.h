
#pragma once

#include "ph_pch.h"


#define PH_LOG_TRACE 0
#define PH_LOG_INFO 1
#define PH_LOG_WARNING 2
#define PH_LOG_ERROR 3
#define PH_LOG_FATAL 4

#ifdef PH_DEBUG
	#define ph_log_level(x) Phoenix::CMD::level = x;

	#define ph_log(x) std::cout << Phoenix::CMD::get_time() << " " << x << "\n";

	#define ph_internal_trace(x) if(Phoenix::CMD::level <= 0){ std::cout << Phoenix::CMD::style.dim << Phoenix::CMD::get_time() << " <Phoenix> " << x << Phoenix::CMD::style.reset << "\n"; }
	#define ph_internal_info(x) if(Phoenix::CMD::level <= 1){ std::cout << Phoenix::CMD::color.cyan << Phoenix::CMD::get_time() << " <Phoenix> " << x << Phoenix::CMD::style.reset << "\n"; }
	#define ph_internal_warning(x) if(Phoenix::CMD::level <= 2){ std::cout << Phoenix::CMD::color.yellow << Phoenix::CMD::get_time() << " <Phoenix> " << x << Phoenix::CMD::style.reset << "\n"; }
	#define ph_internal_error(x) if(Phoenix::CMD::level <= 3){ std::cout << Phoenix::CMD::color.red << Phoenix::CMD::get_time() << " <Phoenix> " << x << Phoenix::CMD::style.reset << "\n"; }
	#define ph_internal_fatal(x) std::cout << Phoenix::CMD::color.black << Phoenix::CMD::backgroundColor.red << Phoenix::CMD::get_time() << " <Phoenix> " << x << Phoenix::CMD::style.reset << "\n";

	#define ph_trace(x) if(Phoenix::CMD::level <= 0){ std::cout << Phoenix::CMD::style.dim << Phoenix::CMD::get_time() << " " << x << Phoenix::CMD::style.reset << "\n"; }
	#define ph_info(x) if(Phoenix::CMD::level <= 1){ std::cout << Phoenix::CMD::color.cyan << Phoenix::CMD::get_time() << " " << x << Phoenix::CMD::color.white << "\n"; }
	#define ph_warning(x) if(Phoenix::CMD::level <= 2){ std::cout << Phoenix::CMD::color.yellow << Phoenix::CMD::get_time() << " " << x << Phoenix::CMD::color.white << "\n"; }
	#define ph_error(x) if(Phoenix::CMD::level <= 3){ std::cout << Phoenix::CMD::color.red << Phoenix::CMD::get_time() << " " << x << Phoenix::CMD::color.white << "\n"; }
	#define ph_fatal(x) std::cout << Phoenix::CMD::backgroundColor.red << Phoenix::CMD::get_time() << " " << x << Phoenix::CMD::style.reset << "\n";


	#define ph_internal_assert(x, y) if(!(x)){ph_internal_error(y)}
	#define ph_assert(x, y) if(!(x)){ph_error(y)}
#else
	#define ph_log_level(x)

	#define ph_log(x)

	#define ph_internal_trace(x)
	#define ph_internal_info(x)
	#define ph_internal_warning(x)
	#define ph_internal_error(x)
	#define ph_internal_fatal(x)

	#define ph_trace(x)
	#define ph_info(x)
	#define ph_warning(x)
	#define ph_error(x)
	#define ph_fatal(x)
#endif



namespace Phoenix::CMD{
	

	struct Color{
		std::string black =	  "\x1b[30m";
		std::string red =	  "\x1b[31m";
		std::string yellow =  "\x1b[33m";
		std::string green =   "\x1b[32m";
		std::string blue = 	  "\x1b[34m";
		std::string magenta = "\x1b[35m";
		std::string cyan = 	  "\x1b[36m";
		std::string white =   "\x1b[37m";
	};

	struct BackgroundColor {
		std::string black = "\x1b[40m";
		std::string red = "\x1b[41m";
		std::string yellow = "\x1b[43m";
		std::string green = "\x1b[42m";
		std::string blue = "\x1b[44m";
		std::string magenta = "\x1b[45m";
		std::string cyan = "\x1b[46m";
		std::string white = "\x1b[47m";
	};

	struct Style {
		std::string reset = 	"\x1b[0m";
		std::string dim = 		"\x1b[90m";
		std::string underline = "\x1b[4m";
	};

	static Color color;
	static BackgroundColor backgroundColor;
	static Style style;


	static int level = 0;
	std::string get_time();

	void static init(){
		system("cls");
		ph_internal_info("Logging Initialized");
	};
}





