#pragma once


namespace Phoenix{
	
	class AssetManager;
	class Camera;


	struct Renderer2DPerformanceMetrics{
		unsigned int drawCalls = 0;
		unsigned int verticies = 0;
		unsigned int indicies = 0;
	};

	class Renderer2D{
		public:
			Renderer2D(AssetManager* asset_manager);
			~Renderer2D() = default;
			
			void init();


			void bind();

			void drawQuad(uint32_t entt_id, glm::mat4& transform, glm::vec4& color);
			void drawQuad(uint32_t entt_id, glm::mat4& transform, glm::vec4& color, UUID& texture);

			void resetPerfMetrics();

		public:
			Renderer2DPerformanceMetrics performanceMetrics;

		private:
			void draw_quad(uint32_t entt_id, glm::mat4& transform, glm::vec4& color);

			
		private:
			UUID _quad_shader;
			AssetManager* _asset_manager;
	
	};

}
