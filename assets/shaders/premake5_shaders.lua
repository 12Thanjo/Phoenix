

function build_shader_compile_command(shader_module, shader_type, out_dir)
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



project "Shaders"
	kind "Makefile"

	targetdir (target.bin)
	objdir    (target.obj)


	buildcommands {
		( build_shader_compile_command("basic2D", "vert", config.location) ),
		( build_shader_compile_command("basic2D", "frag", config.location) ),


		( build_shader_compile_command("basic3D", "vert", config.location) ),
		( build_shader_compile_command("basic3D", "frag", config.location) ),

		( "{ECHO} Compiled Shaders" ),
	}


	rebuildcommands {
		( build_shader_compile_command("basic2D", "vert", config.location) ),
		( build_shader_compile_command("basic2D", "frag", config.location) ),


		( build_shader_compile_command("basic3D", "vert", config.location) ),
		( build_shader_compile_command("basic3D", "frag", config.location) ),

		( "{ECHO} Compiled Shaders" ),
	}

project "*"
