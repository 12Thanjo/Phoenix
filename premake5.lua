-- premake5

local vulkan_sdk = os.getenv("VULKAN_SDK")

if(vulkan_sdk == nil) then
	premake.error("Failed to find the Vulkan SDK. Please set the environment variable 'VULKAN_SDK' and try again")
end


workspace "Phoenix"
	architecture "x64"

	configurations{
		"Debug",
		"Dev",
		"Optimize",
		"Release",
		"ReleaseDist",
	}

	platforms {
		"Windows",
		"Linux",
	}
	defaultplatform "Windows"


	flags{
		"MultiProcessorCompile",
		-- "NoPCH",
	}


	startproject "Playground"


	---------------------------------------
	-- platform

	filter "system:Windows"
		system "windows"
	filter {}


	filter "system:Linux"
		system "linux"
	filter {}




	------------------------------------------------------------------------------
	-- configs

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"

		defines{
			"_DEBUG",
		}
	filter {}


	filter "configurations:Dev"
		runtime "Debug"
		symbols "On"
		optimize "Off"

		defines{
			"_DEBUG",
		}
	filter {}


	filter "configurations:Optimize"
		runtime "Release"
		symbols "On"
		optimize "On"

		defines{
			"NDEBUG",
		}

		flags{
			"LinkTimeOptimization",
		}
	filter {}


	filter "configurations:Release"
		runtime "Release"
		symbols "Off"
		optimize "Full"

		defines{
			"NDEBUG",
		}

		flags{
			"LinkTimeOptimization",
		}
	filter {}



	filter "configurations:ReleaseDist"
		runtime "Release"
		symbols "Off"
		optimize "Full"

		defines{
			"NDEBUG",
		}

		flags{
			"LinkTimeOptimization",
		}
	filter {}



------------------------------------------------------------------------------
-- global variables
	
config = {
	location = ("%{wks.location}"),
	platform = ("%{cfg.platform}"),
	build    = ("%{cfg.buildcfg}"),
	project  = ("%{prj.name}"),

	VulkanSDK = (vulkan_sdk),
}


target = {
	bin = string.format("%s/build/%s/%s/bin/",   config.location, config.platform, config.build),
	lib = string.format("%s/build/%s/%s/lib/%s", config.location, config.platform, config.build, config.project),
	obj = string.format("%s/build/%s/%s/obj/%s", config.location, config.platform, config.build, config.project),
}


lib = {
	libs      = (config.location),

	Evo       = (config.location .. "/extern/Evo/"),
	GLFW      = (config.location .. "/extern/GLFW/include"),
	glm       = (config.location .. "/extern/glm"),
	PhysX     = (config.location .. "/extern/PhysX/physx/include"),
	stb_image = (config.location .. "/extern/stb_image"),
	tinyobj   = (config.location .. "/extern/tinyobjloader"),

	Vulkan    = (vulkan_sdk .. "/Include"),

	Phoenix   = (config.location .. "/Engine/include"),
}



------------------------------------------------------------------------------
-- extern lib projects


include "libs/GLFW/premake5_GLFW.lua"
include "libs/Evo/premake5_Evo.lua"
include "assets/shaders/premake5_shaders.lua"



------------------------------------------------------------------------------
-- project settings

language "C++"
cppdialect "C++20"
exceptionhandling "Off"
allmodulespublic "Off"

---------------------------------------
-- platform

filter "system:Windows"
	defines{
		"PH_PLATFORM_WINDOWS",
	}
filter {}


filter "system:Linux"
	defines{
		"PH_PLATFORM_LINUX",
	}
filter {}



---------------------------------------
-- build

filter "configurations:Debug"
	warnings "High"
	debugdir(config.location)

	debugenvs {
		("PATH=%PATH%;" .. config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug"),
	}

	defines{
		"PH_BUILD_DEBUG",
		"PH_CONFIG_DEBUG",
		"PH_CONFIG_TRACE",
	}

filter {}


filter "configurations:Dev"
	warnings "High"
	debugdir (config.location)

	debugenvs {
		("PATH=%PATH%;" .. config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug"),
	}

	defines{
		"PH_BUILD_DEV",
		"PH_CONFIG_DEBUG",
	}

filter {}


filter "configurations:Optimize"
	debugdir (config.location)

	debugenvs {
		("PATH=%PATH%;" .. config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/checked"),
	}

	defines{
		"PH_BUILD_OPTIMIZE",
		"PH_CONFIG_DEBUG",
	}

filter {}


filter "configurations:Release"
	debugdir (config.location)

	-- TODO: release
	debugenvs {
		("PATH=%PATH%;" .. config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release"),
	}

	defines{
		"PH_BUILD_RELEASE",
		"PH_CONFIG_RELEASE",
	}
filter {}


filter "configurations:ReleaseDist"
	defines{
		"PH_BUILD_DIST",
		"PH_CONFIG_RELEASE",
	}
filter {}




------------------------------------------------------------------------------
-- projects

include "Engine/premake5_engine.lua"
include "Playground/premake5_playground.lua"
