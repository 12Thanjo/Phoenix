-- premake5.lua
workspace "Phoenix"
	architecture "x64"
	startproject "Phoenix Editor"

	configurations {
		"Debug",
		"Release"
	}

	flags{
		"MultiProcessorCompile"
	}


outputdir = "%{cfg.buildcfg}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "Phoenix/lib/GLFW/include"
IncludeDirs["Glad"] = "Phoenix/lib/Glad/include"
IncludeDirs["glm"] = "Phoenix/lib/glm"
IncludeDirs["ImGui"] = "Phoenix/lib/ImGui"
IncludeDirs["stb_image"] = "Phoenix/lib/stb_image"
IncludeDirs["EnTT"] = "Phoenix/lib/EnTT"
IncludeDirs["jx"] = "Phoenix/lib/jx"
-- IncludeDirs["NodeJS"] = "Phoenix/lib/nodejs/src"
-- IncludeDirs["lemon"] = "Phoenix/lib/lemon"

include "Phoenix/lib/GLFW"
include "Phoenix/lib/Glad"
include "Phoenix/lib/ImGui"
include "Phoenix/lib/jx"
-- include "Phoenix/lib/nodejs"
-- include "Phoenix/lib/lemon"

project "Phoenix"
	location "Phoenix"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"


	targetdir ("bin/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "ph_pch.h"
	pchsource "Phoenix/ph_pch.cpp"


	files{
		"%{prj.name}/*.h",
		"%{prj.name}/*.cpp",

		-- core
		"%{prj.name}/core/**.h",
		"%{prj.name}/core/**.cpp",

		-- src
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

	}

	defines{
		"GLFW_INCLUDE_NONE"
	}

	includedirs{
		"Phoenix",
		"Phoenix/src",

		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.Glad}",
		-- "%{IncludeDirs.NodeJS}",
		"%{IncludeDirs.glm}",
		-- "%{IncludeDirs.ImGui}",
		"%{IncludeDirs.stb_image}",
		"%{IncludeDirs.EnTT}",
		"%{IncludeDirs.jx}",
		-- "%{IncludeDirs.lemon}",

		"Phoenix/lib/Glad/inlcude/glad",
	}


	links{
		"GLFW",
		"Glad",
		-- "lemon",
		-- "ImGui",
		"Phoenix/lib/jx/x64/jx.lib",
		"jx",
		-- "NodeJS",
		"opengl32.lib",
		"shlwapi.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"PH_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}


	filter "configurations:Debug"
		defines "PH_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "PH_RELEASE"
	   buildoptions "/MT"
		optimize "on"

--------------------------------------------------------------------------------------------

project "Phoenix Runtime"
	location "Phoenix Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"


	targetdir ("bin/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediates/" .. outputdir .. "/%{prj.name}")


	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs{
		"Phoenix",
		"Phoenix/src",
		-- "%{IncludeDirs.EnTT}",
		"%{IncludeDirs.jx}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.Glad}",
		"%{IncludeDirs.GLFW}",
		-- "%{IncludeDirs.lemon}",
	}


	links{
		"Phoenix",
		"ImGui",
		"GLFW",
		"Glad",
		-- "lemon",
		-- "NodeJS",
	}


	filter "system:windows"
		systemversion "latest"

		defines {
			"PH_PLATFORM_WINDOWS"
		}

		postbuildcommands {
			-- ('echo -------------$(SolutionDir)bin/bin/' .. outputdir .. '/%{prj.name}'),
			('{COPYDIR} "$(SolutionDir)/Phoenix/lib/jx/x64" "$(SolutionDir)bin/bin/' .. outputdir .. '/%{prj.name}"')
		}

	filter "configurations:Debug"
		defines "PH_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "PH_RELEASE"
		optimize "on"
		kind "WindowedApp"
	   buildoptions "/MT"

--------------------------------------------------------------------------------------------

project "Phoenix Editor"
	location "Phoenix Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"


	targetdir ("bin/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediates/" .. outputdir .. "/%{prj.name}")


	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs{
		"Phoenix",
		"Phoenix/src",
		-- "%{IncludeDirs.EnTT}",
		"%{IncludeDirs.jx}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.Glad}",
		"%{IncludeDirs.GLFW}",
		-- "%{IncludeDirs.lemon}",
	}


	links{
		"Phoenix",
		"ImGui",
		"GLFW",
		"Glad",
		-- "lemon",
		-- "NodeJS",
	}



	filter "system:windows"
		systemversion "latest"

		defines {
			"PH_PLATFORM_WINDOWS"
		}

		postbuildcommands {
			-- ('echo -------------$(SolutionDir)%{cfg.targetdir.relpath}'),
			('{COPYDIR} "$(SolutionDir)/Phoenix/lib/jx/x64" "$(SolutionDir)bin/bin/' .. outputdir .. '/%{prj.name}"')
		}

	filter "configurations:Debug"
		defines "PH_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "PH_RELEASE"
		optimize "on"
		kind "WindowedApp"
		symbols "off"
		buildoptions "/MT"





