#pragma once



namespace Phoenix{
	class Window;

	struct FileDialogsConfig{
		const char* title;
		std::map<std::string, std::string> filters = {{"All Files (*.*)", ""}};
		std::string filepath = "";
	};
	
	class FileDialogs{
		public:
			FileDialogs() = default;
			~FileDialogs() = default;


			static std::string open(Window& window, FileDialogsConfig config);
			static std::string save(Window& window, FileDialogsConfig config);
	};

}
