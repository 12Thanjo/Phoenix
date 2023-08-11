-- premake5.lua

project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "Off"
	
	targetdir(target.lib)
	objdir(target.obj)


	includedirs{
		(lib.GLFW),
	}


	local src_dir = ("../../extern/GLFW/src/");

	-- location (config.location .. "/extern/GLFW/")
	files {
		(src_dir .. "context.c"),
		(src_dir .. "init.c"),
		(src_dir .. "input.c"),
		(src_dir .. "monitor.c"),
		(src_dir .. "null_init.c"),
		(src_dir .. "null_joystick.c"),
		(src_dir .. "null_monitor.c"),
		(src_dir .. "null_window.c"),
		(src_dir .. "osmesa_context.c"),
		(src_dir .. "platform.c"),
		(src_dir .. "vulkan.c"),
		(src_dir .. "window.c"),
	}



	defines{ 
		"_CRT_SECURE_NO_WARNINGS",
	}

	warnings "Off"


	------------------------------------------
	-- OS

	filter "system:Windows"
		defines{
			"_GLFW_WIN32",
		}

		files {
			(src_dir .. "egl_context.c"),
			(src_dir .. "wgl_context.c"),

			(src_dir .. "win32_init.c"),
			(src_dir .. "win32_joystick.c"),
			(src_dir .. "win32_module.c"),
			(src_dir .. "win32_monitor.c"),
			(src_dir .. "win32_thread.c"),
			(src_dir .. "win32_time.c"),
			(src_dir .. "win32_window.c"),
		}
	filter{}

	filter "system:Linux"
		defines{
			"_GLFW_X11",
		}

		files {
			-- TODO...
		}
	filter{}



project "*"