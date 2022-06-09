#pragma once



namespace Phoenix{
	class Window;
	
	class FileDialogs{
		public:
			FileDialogs() = default;
			~FileDialogs() = default;


			static std::string open(Window& window, const char* filter);
			static std::string save(Window& window, const char* filter);
	};

}
