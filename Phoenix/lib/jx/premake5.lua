
-- git checkout tags/3.3

project "jx"
	kind "StaticLib"
	language "C++"
	-- cppdialect "C++20"
	staticruntime "on"
	
	targetdir ("x64")
	objdir ("bin-int")
