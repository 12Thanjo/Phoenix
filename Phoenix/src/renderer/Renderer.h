#pragma once


namespace Phoenix{
	
	enum class RendererAPI{
		None = 0, OpenGL = 1
	};

	class Renderer{
		private:
			static RendererAPI renderer_api;
	
		public:
			inline static RendererAPI getAPI() { return renderer_api; }
			
	};

}
