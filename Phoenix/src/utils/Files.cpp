#include "ph_pch.h"
#include "Files.h"

#include <windows.h>
#include <stdio.h>
#include "Shlwapi.h"

#define FILE_PATH_REF 	   const std::string& path
#define ERROR_CALLBACK std::function<void(std::string)> error_callback

#define RUN_ERROR_CHECK(func) try{ \
			return func; \
		}catch(const fs::filesystem_error& err){ \
			error_callback(err.what()); \
			return false; \
		}



namespace Files{

	namespace fs = std::filesystem;
	
	
	std::string readFile(FILE_PATH_REF){
		std::ifstream file(path);
		std::stringstream stream;

		std::string line;
		while(getline(file, line)){
			stream << line << "\n";
		};

		return stream.str();
	}

	void writeFile(FILE_PATH_REF, const std::string& output){
		PH_ASSERT(directoryExists(getFilePath(path)), "Invalid Path\n\t" << path);

		std::ofstream fout(path);
		fout << output;
	}




	bool deleteFile(FILE_PATH_REF, ERROR_CALLBACK){
		RUN_ERROR_CHECK(fs::remove(path));
	}

	bool copyFile(FILE_PATH_REF, const std::string& destination, ERROR_CALLBACK){
		RUN_ERROR_CHECK(fs::copy_file(path, destination));
	}

	bool renameFile(FILE_PATH_REF, const std::string& name, ERROR_CALLBACK){
		try{ 
			fs::rename(path, name);
			return true;
		}catch(const fs::filesystem_error& err){
			error_callback(err.what());
			return false;
		}
	}


	bool fileExists(FILE_PATH_REF, ERROR_CALLBACK){
		RUN_ERROR_CHECK(fs::exists(path));
	}

	bool directoryExists(FILE_PATH_REF){
		return PathFileExistsA(path.c_str()) == 1;
	}



	bool createDirectory(FILE_PATH_REF, ERROR_CALLBACK){
		RUN_ERROR_CHECK(fs::create_directories(path));
	}

	bool deleteDirectory(FILE_PATH_REF, ERROR_CALLBACK){
		RUN_ERROR_CHECK(fs::remove_all(path));
	}



	void directoryIterator(std::string path, std::function<void(const std::string&, bool)> func){
		for(auto& dir : fs::directory_iterator(path)){
			func(dir.path().string(), dir.is_directory());
		}
	}


	//////////////////////////////////////////////////////////////////////
	// path manipulation


	std::string getFileExtention(std::string path){
		size_t dot_index = path.find_last_of('.');
		size_t forward = path.find_last_of('/');
		size_t backward = path.find_last_of('\\');
		if(dot_index < forward || dot_index < backward){
		    return path.substr(dot_index + 1);
		}else{
		    return "";
		}
	}

	std::string getFileName(std::string path){
	    size_t forward = path.find_last_of('/');
	    size_t backward = path.find_last_of('\\');
	    if(forward < backward){
	    	return path.substr(forward+1);
	    }else{
	    	return path.substr(backward+1);
	    }
	}


	std::string getFileNameWithoutExtention(std::string path){
		std::string name = getFileName(path);
		std::string ext = getFileExtention(path);
		if(ext != ""){
		    return name.substr(0, name.length() - ext.length() - 1);
		}else{
		    return name;
		}
	}


	std::string normalize(std::string path, bool forwards){
		if(forwards == true){
			std::replace( path.begin(), path.end(), '/', '\\');
		}else{
			std::replace( path.begin(), path.end(), '\\', '/');
		}

		return path;
	}


	std::string getFilePath(std::string path){
	    if(getFileExtention(path) != ""){
	        return path.substr(0, normalize(path).find_last_of('\\'));
	    }else{
	        return path;
	    };
	}


	std::string getFilePathUpDirectory(std::string path, int number){
		if(getFileExtention(path) != ""){
		    number += 1;
		}

		path = normalize(path);

		for(int i = 0; i < number; i += 1){
			char last_char = path[path.length()-1];
		    if(last_char == '/' || last_char == '\\'){
		        path = path.substr(0, path.length()-1);      
		    }
		    path = path.substr(0, path.find_last_of("/"));
		}

		return path;
	}


	std::string relative(std::string path, std::string relative){
		if(path.substr(0, relative.length()) == relative){
			return path.substr(relative.length());
		}else{
			return path;
		};
	}

	
	//////////////////////////////////////////////////////////////////////


	std::string getFileCreationTime(FILE_PATH_REF){
		std::wstring widestr = std::wstring(path.begin(), path.end());
		const wchar_t* widecstr = widestr.c_str();

		HANDLE hFile1;
	    FILETIME ftCreate;
	    SYSTEMTIME stUTC;
	    hFile1 = CreateFile(widecstr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if(hFile1 == INVALID_HANDLE_VALUE)
		{
		    printf("Could not open file, error %ul\n", GetLastError());
		    // return -1;
		}

		if(!GetFileTime(hFile1, &ftCreate, NULL, NULL))
		{
		    printf("Something wrong!\n");
		    // return FALSE;
		}

		FileTimeToSystemTime(&ftCreate, &stUTC);
		// printf("UTC System Time format:\n");
		// printf("Created on: %02d/%02d/%d %02d:%02d\n", stUTC.wDay, stUTC.wMonth, stUTC.wYear, stUTC.wHour, stUTC.wMinute);

		std::string output =
			std::to_string(stUTC.wYear)   + "," +
			std::to_string(stUTC.wMonth)  + "," +
			std::to_string(stUTC.wDay)    + "," +
			std::to_string(stUTC.wHour)   + "," +
			std::to_string(stUTC.wMinute) + "," +
			std::to_string(stUTC.wSecond) + "," +
			std::to_string(stUTC.wMilliseconds);
			

		return output;
	}



	void openInDefaultProgram(FILE_PATH_REF){
		ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}


}


#undef ERROR_CALLBACK