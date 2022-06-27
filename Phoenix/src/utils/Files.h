#pragma once

#define FILE_PATH_REF const std::string& path
#define ERROR_CALLBACK std::function<void(std::string)> error_callback = [](std::string err){}

namespace Files{

	//////////////////////////////////////////////////////////////////////
	// file operations

	std::string readFile(FILE_PATH_REF);
	void writeFile(FILE_PATH_REF, const std::string& output);

	bool deleteFile(FILE_PATH_REF, ERROR_CALLBACK);
	bool copyFile(FILE_PATH_REF, const std::string& destination, ERROR_CALLBACK);
	bool renameFile(FILE_PATH_REF, const std::string& name, ERROR_CALLBACK);
	
	bool fileExists(FILE_PATH_REF, ERROR_CALLBACK);
	bool directoryExists(FILE_PATH_REF);

	bool createDirectory(FILE_PATH_REF, ERROR_CALLBACK); // supports nested
	bool deleteDirectory(FILE_PATH_REF, ERROR_CALLBACK); // supports nested


	//////////////////////////////////////////////////////////////////////
	// path manipulation

	std::string getFileExtention(std::string path);
	std::string getFileName(std::string path);
	std::string getFileNameWithoutExtention(std::string path);
	std::string normalize(std::string path, bool forwards = true);
	std::string getFilePath(std::string path);
	std::string getFilePathUpDirectory(std::string path, int number = 1);

	//////////////////////////////////////////////////////////////////////
	
	std::string getFileCreationTime(FILE_PATH_REF);
	void openInDefaultProgram(FILE_PATH_REF);

}

#undef FILE_PATH_REF
#undef ERROR_CALLBACK
