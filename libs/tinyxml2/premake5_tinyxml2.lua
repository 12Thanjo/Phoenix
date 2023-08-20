-- premake5.lua

project "tinyxml2"
	kind "StaticLib"
	language "C++"
    staticruntime "off"


    targetdir(target.lib)
    objdir(target.obj)



	files{
		(lib.tinyxml2 .. "/tinyxml2.cpp"),
		(lib.tinyxml2 .. "/tinyxml2.h"),
	}



	warnings "Off"


project "*"