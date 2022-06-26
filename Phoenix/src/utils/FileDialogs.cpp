#include "ph_pch.h"
#include "FileDialogs.h"

#include <windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "src/Window.h"


namespace Phoenix{

	std::string get_filter_str(std::map<std::string, std::string>& filters){
		std::string filter_str = "";

		for(auto& filter : filters){
			filter_str += filter.first + '\0' + filter.second + '\0';
		}

		return filter_str;
	}


	std::string FileDialogs::open(Window& window, FileDialogsConfig config){
		std::string filter_str = get_filter_str(config.filters);

		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window(window.getWindowContext());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if(GetCurrentDirectoryA(256, currentDir)){
			ofn.lpstrInitialDir = currentDir;
		}

		ofn.lpstrFilter = filter_str.c_str();
		ofn.lpstrTitle = config.title;

		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if(GetOpenFileNameA(&ofn) == TRUE){
			return ofn.lpstrFile;
		}

		return std::string();
	}


	std::string FileDialogs::save(Window& window, FileDialogsConfig config){
		std::string filter_str = get_filter_str(config.filters);


		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window(window.getWindowContext());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if(GetCurrentDirectoryA(256, currentDir)){
			ofn.lpstrInitialDir = currentDir;
		}

		ofn.lpstrFilter = filter_str.c_str();
		ofn.lpstrTitle = config.title;

		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(ofn.lpstrFilter, '\0') + 1;

		if(GetSaveFileNameA(&ofn) == TRUE){
			return ofn.lpstrFile;
		}
		
		return std::string();
	}
	
}
