-- premake5.lua

project "Evo"
	kind "StaticLib"
	language "C++"
	
	targetdir(target.lib)
	objdir(target.obj)


	files {
		"../../extern/Evo/**.cpp",
		"../../extern/Evo/**.h",
	}

	includedirs{
		"../../extern/Evo",
	}



	------------------------------------------
	-- build

	filter "configurations:Debug"
		defines{
			"EVO_CONFIG_DEBUG",
		}
	filter {}

	filter "configurations:Dev"
		defines{
			"EVO_CONFIG_DEBUG",
		}
	filter {}


	filter "configurations:Optimize"
		defines{
			"EVO_CONFIG_DEBUG",
		}
	filter {}

	filter "configurations:Release"
		defines{
			-- none...
		}
	filter {}


	filter "configurations:ReleaseDist"
		defines{
			-- none...
		}
	filter {}



project "*"