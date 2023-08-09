-- premake5

project "Engine"
	kind "SharedLib"
	
	targetdir(target.bin)
	objdir(target.obj)

	files {
		"src/**.h",
		"src/**.cpp",
	}

	includedirs{
		"./src",
		(lib.Phoenix),

		(lib.libs),
		(lib.Evo),
		(lib.GLFW),
		(lib.glm),
		(lib.Vulkan),
		
		(lib.stb_image),
		(lib.tinyobj),
	}


	defines {
		"PH_EXPORT",
	}

	links {
		"GLFW",
		"Evo",
	}




	------------------------------------------
	-- OS

	filter "system:Windows"
		links{
			(config.VulkanSDK .. "/Lib/vulkan-1.lib"),
			-- "user32",
			"dwmapi",
		}	
	filter {}

	filter "system:Linux"
		links{
			"vulkan",
			"xcb",
			"X11",
			"X11-xcb",
			"xkbcommon",
			-- "$VULKAN_SDK/lib",
		}

		libdirs{
			-- "/usr/X11R6/lib",
		}
	filter {}


	------------------------------------------
	-- build

	filter "configurations:Debug"
		ignoredefaultlibraries{
			"LIBCMTD"
		}
	filter {}

	filter "configurations:Dev"
		ignoredefaultlibraries{
			"LIBCMTD"
		}
	filter {}


	filter "configurations:Optimize"
		ignoredefaultlibraries{
			"LIBCMT"
		}
	filter {}

	filter "configurations:Release"
		ignoredefaultlibraries{
			"LIBCMT"
		}
	filter {}

	filter "configurations:ReleaseDist"
		ignoredefaultlibraries{
			"LIBCMT"
		}
	filter {}



project "*"