#pragma once


#include "Renderer2D.h"
#include "src/rendering/window.h"



namespace Phoenix{

	struct RendererPerformanceMetrics{
		uint32_t draw_calls = 0;
		uint32_t verticies = 0;
		uint32_t indicies = 0;

		uint32_t draw_calls_2d = 0;
		uint32_t quads = 0;
		uint32_t verticies_2d = 0;
		uint32_t indicies_2d = 0;
	};
	
	class Renderer{
		private:
			// uint32_t vertex_array;
			Renderer2D renderer_2d;
	
		public:
			RendererPerformanceMetrics performanceMetrics;

			Renderer(unsigned int width, unsigned int height);
			~Renderer() = default;

			void begin_scene();
			void render();
			void end_scene();

			void set_camera_projection(float width, float height);

			void draw_quad(const glm::mat4& transform, const glm::vec4& color) {
				renderer_2d.draw_quad(transform, color);
			};

		private:
			void clear_perf_metrics(){
				performanceMetrics.draw_calls = 0;
				performanceMetrics.verticies = 0;
				performanceMetrics.indicies = 0;

				performanceMetrics.draw_calls_2d = 0;
				performanceMetrics.quads = 0;
				performanceMetrics.verticies_2d = 0;
				performanceMetrics.indicies_2d = 0;
			}

			void update_perf_metrics(){
				performanceMetrics.draw_calls = renderer_2d.performanceMetrics.draw_calls;
				performanceMetrics.quads = renderer_2d.performanceMetrics.quads;
				performanceMetrics.verticies = renderer_2d.performanceMetrics.verticies;
				performanceMetrics.indicies = renderer_2d.performanceMetrics.indicies;

				performanceMetrics.draw_calls_2d = renderer_2d.performanceMetrics.draw_calls;
				performanceMetrics.verticies_2d = renderer_2d.performanceMetrics.verticies;
				performanceMetrics.indicies_2d = renderer_2d.performanceMetrics.indicies;
			}
			
	};

}

