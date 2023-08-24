

local function build_shader_compile_command(shader_module, shader_type, out_dir)
	-- TODO: Add Linux compiler
	return (
		config.VulkanSDK .. "/bin/glslc.exe"
		.. " -O " -- optimize
		.. " -fshader-stage=" .. shader_type .. " "
		.. " -x glsl " -- make sure GLSL is used (not HLSL)
		.. config.location .. "/assets/shaders/" .. shader_module .. "." .. shader_type .. ".glsl"
		.. " -o " .. out_dir .. "assets/shaders/" .. shader_module .. "." .. shader_type .. ".spv"
	)
end



local function compile_shaders(location, shader_modules)

	for i, module in ipairs(shader_modules) do
		buildcommands{
			( build_shader_compile_command(module, "vert", location) ),
			( build_shader_compile_command(module, "frag", location) ),
		}

		rebuildcommands{
			( build_shader_compile_command(module, "vert", location) ),
			( build_shader_compile_command(module, "frag", location) ),
		}
	end

end



project "Shaders"
	kind "Makefile"

	targetdir (target.bin)
	objdir    (target.obj)


	buildcommands {
		( "{ECHO} === Compiled Shaders === " ),
	}


	rebuildcommands {
		( "{ECHO} === Compiled Shaders === " )
	}


	compile_shaders(config.location, {
		"basic3D",

		"basic2D",
		"text2D",
	})


project "*"
