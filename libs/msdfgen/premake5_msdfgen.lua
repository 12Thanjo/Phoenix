-- premake5.lua

project "msdfgen"
	kind "StaticLib"
	language "C++"
    staticruntime "off"


    targetdir(target.lib)
    objdir(target.obj)


    includedirs{
    	(lib.msdfgen),
    	(lib.freetype),

    	(lib.lodepng),
    	(lib.tinyxml2),
    }


	files{
		(lib.msdfgen .. "/core/**.h"),
		(lib.msdfgen .. "/core/**.hpp"),
		(lib.msdfgen .. "/core/**.cpp"),

		(lib.msdfgen .. "/ext/**.h"),
		(lib.msdfgen .. "/ext/**.hpp"),
		(lib.msdfgen .. "/ext/**.cpp"),

		(lib.msdfgen .. "/msdfgen.h"),
		(lib.msdfgen .. "/msdfgen-ext.h"),
	}


	defines{
		"MSDFGEN_USE_CPP11",

		"MSDFGEN_USE_LODEPNG",
	}

	links{
		"freetype",
		"lodepng",
		"tinyxml2",
	}

	warnings "Off"

project "*"