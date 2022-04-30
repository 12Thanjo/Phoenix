-- premake5.lua
workspace "Phoenix"
	architecture "x64"
	startproject "Editor"

	configurations {
		"Debug",
		"Release"
	}


outputdir = "%{cfg.buildcfg}/%{cfg.system}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "Phoenix/lib/GLFW/include"
IncludeDirs["Glad"] = "Phoenix/lib/Glad/include"
IncludeDirs["glm"] = "Phoenix/lib/glm"
IncludeDirs["ImGui"] = "Phoenix/lib/ImGui"

include "Phoenix/lib/GLFW"
include "Phoenix/lib/Glad"
include "Phoenix/lib/ImGui"

project "Phoenix"
	location "Phoenix"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ph_pch.h"
	pchsource "Phoenix/ph_pch.cpp"

	files{
		"%{prj.name}/*.h",
		"%{prj.name}/*.cpp",

		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/platform/**.h",
		"%{prj.name}/platform/**.cpp",

		"%{prj.name}/lib/glm/glm/**.hpp",
		"%{prj.name}/lib/glm/glm/**.inl",
	}

	includedirs{
		"Phoenix",
		"Phoenix/src",

		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.Glad}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.ImGui}",
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"PH_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}


	filter "configurations:Debug"
		defines "PH_DEBUG"
		-- buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "PH_RELEASE"
		-- buildoptions "/MD"
		optimize "on"

	filter { "system:windows", "configurations:Release" }
	   buildoptions "/MT"


project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs{
		"Phoenix"
	}


	links{
		"Phoenix"
	}


	filter "system:windows"
		systemversion "latest"

		defines {
			"PH_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "PH_DEBUG"
		-- buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "PH_RELEASE"
		-- buildoptions "/MD"
		optimize "on"

	filter { "system:windows", "configurations:Release" }
	   buildoptions "/MT"
