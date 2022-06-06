#include "ph_pch.h"
#include "shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Phoenix{

	int compile_shader(const std::string& vertex_source, const std::string& fragment_source){
		// create vertex shader
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		const char* source = vertex_source.c_str();
		glShaderSource(vertex_shader, 1, &source, 0);
		glCompileShader(vertex_shader);

		// make sure vertex shader compiled successfully
		GLint compiled_successfully = 0;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled_successfully);
		if(compiled_successfully == GL_FALSE){
			GLint maxLength = 0;
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(vertex_shader, maxLength, &maxLength, &infoLog[0]);
			
			glDeleteShader(vertex_shader);
			PH_WARNING("Vertex shader compilation failure:\n\t" << infoLog.data());

			return -1;
		}
		

		// create fragment shader
		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		source = fragment_source.c_str();
		glShaderSource(fragment_shader, 1, &source, 0);
		glCompileShader(fragment_shader);


		// make sure fragment shader compiled successfully
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled_successfully);
		if(compiled_successfully == GL_FALSE){
			GLint maxLength = 0;
			glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(fragment_shader, maxLength, &maxLength, &infoLog[0]);
			
			glDeleteShader(fragment_shader);
			glDeleteShader(vertex_shader);
			PH_WARNING("Fragment shader compilation failure:\n\t" << infoLog.data());

			return -1;
		}


		// create shader program
		int shader_id = glCreateProgram();
		glAttachShader(shader_id, vertex_shader);
		glAttachShader(shader_id, fragment_shader);
		glLinkProgram(shader_id);

		

		// make sure shader program linked successfully
		GLint linked_successfully = 0;
		glGetProgramiv(shader_id, GL_LINK_STATUS, (int *)&linked_successfully);
		if(linked_successfully == GL_FALSE){
			GLint maxLength = 0;
			glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(shader_id, maxLength, &maxLength, &infoLog[0]);
			
			glDeleteProgram(shader_id);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
			PH_WARNING("Shader program linking failure:\n\t" << infoLog.data());

			return -1;
		}


		glDetachShader(shader_id, vertex_shader);
		glDetachShader(shader_id, fragment_shader);


		return shader_id;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	Shader::Shader(std::string vertex_source, std::string fragment_source){
		_id = compile_shader(vertex_source, fragment_source);
		PH_LOG("Compiled Shader (from string)");
	}

	Shader::Shader(std::string filepath){
		std::ifstream stream(filepath);

		enum class ShaderType {
			NONE = -1,
			VERTEX = 0,
			FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while(getline(stream, line)){
			if(line.find("#shader") != std::string::npos){
				if(line.find("vertex") != std::string::npos){
					type = ShaderType::VERTEX;
				}else if(line.find("fragment") != std::string::npos){
					type = ShaderType::FRAGMENT;
				}
			}else{
				ss[(int)type] << line << "\n";
			}
		}

		// std::cout << "Vertex: " << std::endl;
		// std::cout << ss[0].str() << std::endl;
		// std::cout << std::endl;	
		// std::cout << "---------------------\nFragment: " << std::endl;
		// std::cout << ss[1].str() << std::endl;

		_id = compile_shader(ss[0].str(), ss[1].str());
		if(_id != -1){
			// PH_LOG("Compiled Shader: " << filepath << "(glID: " << _id << ")");
		}else{
			PH_WARNING("Shader Compilation Failed: " << filepath);
		}
	}

	Shader::~Shader(){
		glDeleteProgram(_id);
	}


	void Shader::bind(){
		glUseProgram(_id);
	}

	void Shader::unbind(){
		glUseProgram(0);
	}



	void Shader::uploadFloat(const std::string& name, const float& value){
		GLint location = get_uniform_location(name);

		PH_ASSERT(location != -1, "Invalid uniform name (" << name << ")");

		glUniform1f(location, value);
	}


	void Shader::uploadFloat2(const std::string& name, const glm::vec2& values){
		GLint location = get_uniform_location(name);

		PH_ASSERT(location != -1, "Invalid uniform name (" << name << ")");

		glUniform2f(location, values.x, values.y);
	}


	void Shader::uploadFloat3(const std::string& name, const glm::vec3& values){
		GLint location = get_uniform_location(name);

		PH_ASSERT(location != -1, "Invalid uniform name (" << name << ")");

		glUniform3f(location, values.x, values.y, values.z);
	}

	void Shader::uploadFloat4(const std::string& name, const glm::vec4& values){
		GLint location = get_uniform_location(name);

		PH_ASSERT(location != -1, "Invalid uniform name (" << name << ")");

		glUniform4f(location, values.x, values.y, values.z, values.w);
	}


	void Shader::uploadMat4(const std::string& name, const glm::mat4& matrix){
		GLint location = get_uniform_location(name);

		PH_ASSERT(location != -1, "Invalid uniform name (" << name << ")");

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::uploadInt(const std::string& name, const int& value){
		GLint location = get_uniform_location(name);

		PH_ASSERT(location != -1, "Invalid uniform name (" << name << ")");

		glUniform1i(location, value);
	}



	int Shader::get_uniform_location(const std::string& name){
		if(uniform_location_cache.find(name) != uniform_location_cache.end()){
			return uniform_location_cache[name];
		}

		int location = glGetUniformLocation(_id, name.c_str());

		if(location == -1){
			PH_WARNING("uniform (" << name << ") doesnt exist");
		}
		

		uniform_location_cache[name] = location;

		return location;
	};

	
}
	