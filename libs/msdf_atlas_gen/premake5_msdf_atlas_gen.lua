-- premake5.lua

project "msdf_atlas_gen"
	kind "StaticLib"
	language "C++"
    staticruntime "off"


    targetdir(target.lib)
    objdir(target.obj)


    includedirs{
    	(lib.msdfgen),
    	(lib.msdf_atlas_gen),

    	(lib.lodepng),
    	(lib.tinyxml2),
    }


	files{
		(lib.msdf_atlas_gen .. "/**.h"),
		(lib.msdf_atlas_gen .. "/**.hpp"),
		(lib.msdf_atlas_gen .. "/**.cpp"),
	}


	defines{
		"_CRT_SECURE_NO_WARNINGS",

		"MSDF_ATLAS_PUBLIC=",
		"MSDF_ATLAS_NO_ARTERY_FONT",
	}

	links{
		"msdfgen",
	}

	warnings "Off"


project "*"