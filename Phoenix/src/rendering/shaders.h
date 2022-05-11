#pragma once


namespace Phoenix{

	class Shader{
		private:
			RendererID renderer_id;
	
		public:
			Shader(const std::string& vertex_source, const std::string& fragment_source);
			~Shader();

			void bind();
			void unbind();


			void upload_uniform_float3(const std::string& name, const glm::vec3& values);
			void upload_uniform_float4(const std::string& name, const glm::vec4& values);
			void upload_uniform_mat4(const std::string& name, const glm::mat4& matrix);
			void upload_uniform_int(const std::string& name, const int& value);


		private:
			void compile(const std::string& vertex_source, const std::string& fragment_source);

	};
	
}
