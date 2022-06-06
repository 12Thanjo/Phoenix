#pragma once

namespace Phoenix{

	class Shader{
		private:
			glID _id;
			std::unordered_map<std::string, int> uniform_location_cache;
	
		public:
			Shader(std::string vertex_source, std::string fragment_source);
			Shader(std::string filepath);
			~Shader();

			void bind();
			void unbind();
			

			void uploadFloat(const std::string& name, const float& value);
			void uploadFloat2(const std::string& name, const glm::vec2& values);
			void uploadFloat3(const std::string& name, const glm::vec3& values);
			void uploadFloat4(const std::string& name, const glm::vec4& values);

			void uploadMat4(const std::string& name, const glm::mat4& matrix);
			void uploadInt(const std::string& name, const int& value);



		private:
			int get_uniform_location(const std::string& name);
	};

}
