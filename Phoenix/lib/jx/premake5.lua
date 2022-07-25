
-- git checkout tags/3.3

project "jx"
	kind "StaticLib"
	language "C++"
	-- cppdialect "C++20"
	staticruntime "on"
	
	targetdir ("bin")
	objdir ("bin-int")


	postbuildcommands {
		-- ('echo -------------$(SolutionDir)bin/bin/' .. outputdir .. '/%{prj.name}'),
		('{COPYDIR} "$(SolutionDir)/Phoenix/lib/jx/x64" "$(SolutionDir)/Phoenix/lib/jx/bin"')
	}
