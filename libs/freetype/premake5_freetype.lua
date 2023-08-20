-- premake5.lua

project "freetype"
	kind "StaticLib"
	language "C"
    staticruntime "off"

	targetdir(target.lib)
	objdir(target.obj)

	includedirs{
		(lib.freetype),
	}

	local freetype_src_path = (config.location .. "/extern/freetype/src")

	files{
		(lib.freetype .. "/ft2build.h"),
		(lib.freetype .. "/freetype/*.h"),
		(lib.freetype .. "/freetype/config/*.h"),
		(lib.freetype .. "/freetype/internal/*.h"),


		(freetype_src_path .. "/autofit/autofit.c"),
		(freetype_src_path .. "/base/ftbase.c"),
		(freetype_src_path .. "/base/ftbbox.c"),
		(freetype_src_path .. "/base/ftbdf.c"),
		(freetype_src_path .. "/base/ftbitmap.c"),
		(freetype_src_path .. "/base/ftcid.c"),
		(freetype_src_path .. "/base/ftdebug.c"),
		(freetype_src_path .. "/base/ftfstype.c"),
		(freetype_src_path .. "/base/ftgasp.c"),
		(freetype_src_path .. "/base/ftglyph.c"),
		(freetype_src_path .. "/base/ftgxval.c"),
		(freetype_src_path .. "/base/ftinit.c"),
		(freetype_src_path .. "/base/ftmm.c"),
		(freetype_src_path .. "/base/ftotval.c"),
		(freetype_src_path .. "/base/ftpatent.c"),
		(freetype_src_path .. "/base/ftpfr.c"),
		(freetype_src_path .. "/base/ftstroke.c"),
		(freetype_src_path .. "/base/ftsynth.c"),
		(freetype_src_path .. "/base/ftsystem.c"),
		(freetype_src_path .. "/base/fttype1.c"),
		(freetype_src_path .. "/base/ftwinfnt.c"),
		(freetype_src_path .. "/bdf/bdf.c"),
		(freetype_src_path .. "/bzip2/ftbzip2.c"),
		(freetype_src_path .. "/cache/ftcache.c"),
		(freetype_src_path .. "/cff/cff.c"),
		(freetype_src_path .. "/cid/type1cid.c"),
		(freetype_src_path .. "/gzip/ftgzip.c"),
		(freetype_src_path .. "/lzw/ftlzw.c"),
		(freetype_src_path .. "/pcf/pcf.c"),
		(freetype_src_path .. "/pfr/pfr.c"),
		(freetype_src_path .. "/psaux/psaux.c"),
		(freetype_src_path .. "/pshinter/pshinter.c"),
		(freetype_src_path .. "/psnames/psnames.c"),
		(freetype_src_path .. "/raster/raster.c"),
		(freetype_src_path .. "/sdf/sdf.c"),
		(freetype_src_path .. "/sfnt/sfnt.c"),
		(freetype_src_path .. "/smooth/smooth.c"),
		(freetype_src_path .. "/svg/ftsvg.c"),
		(freetype_src_path .. "/truetype/truetype.c"),
		(freetype_src_path .. "/type1/type1.c"),
		(freetype_src_path .. "/type42/type42.c"),
		(freetype_src_path .. "/winfonts/winfnt.c"),
	}


	defines{
		"FT2_BUILD_LIBRARY",
		"_CRT_SECURE_NO_WARNINGS",
		"_CRT_NONSTDC_NO_WARNINGS",
	}


	warnings "Off"


project "*"