#include "ph_pch.h"
#include "Files.h"

#include <windows.h>
#include <stdio.h>

namespace Phoenix::Utils{
	
	static std::string getFileCreationTime(std::string filepath){
		std::wstring widestr = std::wstring(filepath.begin(), filepath.end());
		const wchar_t* widecstr = widestr.c_str();

		HANDLE hFile1;
	    FILETIME ftCreate;
	    SYSTEMTIME stUTC;
	    hFile1 = CreateFile(widecstr, GENERIC_READ, FILE_SHARE_READ,  NULL,  OPEN_EXISTING,  FILE_ATTRIBUTE_NORMAL, NULL);

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
	
}
