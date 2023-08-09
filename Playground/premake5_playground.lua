-- premake5

project "Playground"
	kind "ConsoleApp"
	staticruntime "On"
	

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
		(lib.glm),
	}


	links{
		"Engine"
	}



project "*"