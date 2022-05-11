
#ifdef PH_DEBUG
	#undef PH_LOG
	#undef PH_INFO
	#undef PH_WARNING
	#undef PH_ERROR
	#undef PH_FATAL

	#define PH_LOG(x) if(Phoenix::_logging::level <= 0){ std::cout << Phoenix::_logging::style.dim << Phoenix::_logging::get_time() << " " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_INFO(x) if(Phoenix::_logging::level <= 1){ std::cout << Phoenix::_logging::color.cyan << Phoenix::_logging::get_time() << " " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_WARNING(x) if(Phoenix::_logging::level <= 2){ std::cout << Phoenix::_logging::color.yellow << Phoenix::_logging::get_time() << " " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_ERROR(x) if(Phoenix::_logging::level <= 3){ std::cout << Phoenix::_logging::color.red << Phoenix::_logging::get_time() << " " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_FATAL(x) std::cout << Phoenix::_logging::backgroundColor.red << Phoenix::_logging::get_time() << " " << x << Phoenix::_logging::style.reset << "\n";
#endif