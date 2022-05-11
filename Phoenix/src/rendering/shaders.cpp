#include "ph_pch.h"
#include "shaders.h"

#include <glad/glad.h>

namespace Phoenix{
	
	Shader::Shader(const std::string& vertex_source, const std::string& fragment_source){
		compile(vertex_source, fragment_source);
	}

	Shader::~Shader(){
		glDeleteProgram(renderer_id);
	}

	void Shader::compile(const std::string& vertex_source, const std::string& fragment_source){
		// create vertex shader
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* source = vertex_source.c_str();
		glShaderSource(vertex_shader, 1, &source, 0);
		glCompileShader(vertex_shader);

		// make sure vertex shader compiled successfully
		GLint compiled_successfully = 0;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled_successfully);
		if(compiled_successfully == GL_FALSE){
			GLint maxLength = 0;
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertex_shader, maxLength, &maxLength, &infoLog[0]);
			
			glDeleteShader(vertex_shader);
			PH_ERROR("Vertex shader compilation failure:\n\t" << infoLog.data());

			return;
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
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragment_shader, maxLength, &maxLength, &infoLog[0]);
			
			glDeleteShader(fragment_shader);
			glDeleteShader(vertex_shader);
			PH_ERROR("Fragment shader compilation failure:\n\t" << infoLog.data());

			return;
		}


		// create shader program
		renderer_id = glCreateProgram();
		glAttachShader(renderer_id, vertex_shader);
		glAttachShader(renderer_id, fragment_shader);
		glLinkProgram(renderer_id);

		

		// make sure shader program linked successfully
		GLint linked_successfully = 0;
		glGetProgramiv(renderer_id, GL_LINK_STATUS, (int *)&linked_successfully);
		if(linked_successfully == GL_FALSE){
			GLint maxLength = 0;
			glGetProgramiv(renderer_id, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(renderer_id, maxLength, &maxLength, &infoLog[0]);
			
			glDeleteProgram(renderer_id);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
			PH_ERROR("Shader program linking failure:\n\t" << infoLog.data());

			return;
		}


		glDetachShader(renderer_id, vertex_shader);
		glDetachShader(renderer_id, fragment_shader);
	}



	void Shader::bind(){
		glUseProgram(renderer_id);
	}

	void Shader::unbind(){
		glUseProgram(0);
	}




	void Shader::upload_uniform_float3(const std::string& name, const glm::vec3& values){
		GLint location = glGetUniformLocation(renderer_id, name.c_str());

		PH_ASSERT(location != -1, "Invalid uniform name (" << name << ")");

		glUniform3f(location, values.x, values.y, values.z);
	}

	void Shader::upload_uniform_float4(const std::string& name, const glm::vec4& values){
		GLint location = glGetUniformLocation(renderer_id, name.c_str());

		PH_ASSERT(location != -1, "Invalid uniform name (" << name << ")");

		glUniform4f(location, values.x, values.y, values.z, values.w);
	}


	void Shader::upload_uniform_mat4(const std::string& name, const glm::mat4& matrix){
		GLint location = glGetUniformLocation(renderer_id, name.c_str());

		PH_ASSERT(location != -1, "Invalid uniform name (" << name << ")");

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::upload_uniform_int(const std::string& name, const int& value){
		GLint location = glGetUniformLocation(renderer_id, name.c_str());

		PH_ASSERT(location != -1, "Invalid uniform name (" << name << ")");

		glUniform1i(location, value);
	}

}
