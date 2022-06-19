
-- git checkout tags/3.3

project "jx"
	kind "StaticLib"
	language "C++"
	-- cppdialect "C++20"
	staticruntime "on"
	
	targetdir ("bin/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediates/" .. outputdir .. "/%{prj.name}")

	files{
		"**.h",
		"**.cpp",
		-- "jxcore.cpp"
	}


	libdirs {
		"./x64"
	}

	defines{
		"JS_ENGINE_V8",
		"JXCORE_EMBEDDED",
		"WIN64",
		"NDEBUG",
		"_CONSOLE",
		"_LIB",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter { "system:windows", "configurations:Release"}
		buildoptions "/MT"
