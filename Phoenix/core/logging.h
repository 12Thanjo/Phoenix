
#pragma once


#define PH_LOG_LOG 0
#define PH_LOG_INFO 1
#define PH_LOG_WARNING 2
#define PH_LOG_ERROR 3
#define PH_LOG_FATAL 4

#ifdef PH_DEBUG
	#define PH_LOG_LEVEL(x) Phoenix::_logging::level = x;

	#define PH_LOG(x) if(Phoenix::_logging::level <= 0){ std::cout << Phoenix::_logging::style.dim << Phoenix::_logging::get_time() << " <Phoenix> " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_INFO(x) if(Phoenix::_logging::level <= 1){ std::cout << Phoenix::_logging::color.cyan << Phoenix::_logging::get_time() << " <Phoenix> " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_WARNING(x) if(Phoenix::_logging::level <= 2){ std::cout << Phoenix::_logging::color.yellow << Phoenix::_logging::get_time() << " <Phoenix> " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_ERROR(x) if(Phoenix::_logging::level <= 3){ std::cout << Phoenix::_logging::color.red << Phoenix::_logging::get_time() << " <Phoenix> " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_FATAL(x) std::cout << Phoenix::_logging::color.black << Phoenix::_logging::backgroundColor.red << Phoenix::_logging::get_time() << " <Phoenix> " << x << Phoenix::_logging::style.reset << "\n";
	
	#define PH_TRACE() std::cout << Phoenix::_logging::style.dim << Phoenix::_logging::get_time() << " <Trace> File: " << __FILE__ << "\n                   Line: " << __LINE__ << Phoenix::_logging::style.reset << "\n";

	#define PH_ASSERT(cond, msg) if(!(cond)){ \
			PH_ERROR(msg);\
			PH_TRACE();\
			std::cout << "Press [Enter] to continue running -> ";\
			std::cin.get();\
			std::cout << "Continuing...\n";\
		}
#else
	#define PH_LOG_LEVEL(x)

	#define PH_LOG(x)
	#define PH_INFO(x)
	#define PH_WARNING(x)
	#define PH_ERROR(x)
	#define PH_FATAL(x)

	#define PH_TRACE()

	#define PH_ASSERT(x, y)
#endif



namespace Phoenix::_logging{
	

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
		PH_INFO("Logging Initialized");
	};

	
}





