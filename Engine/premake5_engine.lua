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
		(lib.PhysX),
		(lib.Vulkan),
		
		(lib.stb_image),
		(lib.tinyobj),
	}


	defines {
		"PH_EXPORT",

		-- For physx
		"_SILENCE_CXX20_CISO646_REMOVED_WARNING",
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


	------------------------------------------
	-- physx

	filter {"system:Windows", "configurations:Debug"}
		links{
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysX_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXCommon_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXFoundation_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXExtensions_static_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXCharacterKinematic_static_64"),
		}
	filter {}

	filter {"system:Windows", "configurations:Dev"}
		links{
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysX_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXCommon_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXFoundation_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXExtensions_static_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/debug/PhysXCharacterKinematic_static_64"),
		}
	filter {}


	filter {"system:Windows", "configurations:Optimize"}
		links{
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/checked/PhysX_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/checked/PhysXCommon_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/checked/PhysXFoundation_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/checked/PhysXExtensions_static_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/checked/PhysXCharacterKinematic_static_64"),
		}
	filter {}

	filter {"system:Windows", "configurations:Release"}
		links{
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysX_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXCommon_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXFoundation_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXExtensions_static_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXCharacterKinematic_static_64"),
		}
	filter {}

	filter {"system:Windows", "configurations:ReleaseDist"}
		links{
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysX_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXCommon_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXFoundation_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXExtensions_static_64"),
			(config.location .. "/extern/PhysX/physx/bin/win.x86_64.vc143.md/release/PhysXCharacterKinematic_static_64"),
		}
	filter {}


project "*"