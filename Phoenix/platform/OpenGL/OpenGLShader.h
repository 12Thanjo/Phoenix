#pragma once


#include <string>

namespace Phoenix{
	class OpenGLShader{
		private:
			uint32_t renderer_id;
	
		public:
			OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
			~OpenGLShader();


			void bind() const;
			void unbind() const;
			
	};
}

