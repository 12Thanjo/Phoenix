#pragma once


namespace Phoenix{
	namespace Lights{
		class Directional;
	};
	class AssetManager;
	class PerspectiveCamera;

	struct Renderer3DPerformanceMetrics{
		unsigned int drawCalls = 0;
		unsigned int verticies = 0;
		unsigned int indicies = 0;
	};

	class Renderer3D{
		public:
			Renderer3D(AssetManager* asset_manager);
			~Renderer3D() = default;
		
			void init();

			void drawCube(glm::mat4& transform, glm::vec4& color, PerspectiveCamera& camera, Lights::Directional& sunlight);

			void resetPerfMetrics();

		public:
			Renderer3DPerformanceMetrics performanceMetrics;

		private:
			UUID _basic_shader;
			AssetManager* _asset_manager;		
	};

}
