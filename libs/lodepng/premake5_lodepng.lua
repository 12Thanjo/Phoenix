-- premake5.lua

project "lodepng"
	kind "StaticLib"
	language "C++"
    staticruntime "off"


    targetdir(target.lib)
    objdir(target.obj)



	files{
		(lib.lodepng .. "/lodepng.cpp"),
		(lib.lodepng .. "/lodepng.h"),
	}



	warnings "Off"

project "*"