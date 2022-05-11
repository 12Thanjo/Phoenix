#pragma once

#include <glad/glad.h>


#include "src/rendering/buffers.h"
#include "src/rendering/shaders.h"
#include "src/rendering/cameras.h"



namespace Phoenix{

	struct QuadVertex{
		glm::vec4 position;
		glm::vec4 color;
	};

	struct Renderer2DPerformanceMetrics{
		uint32_t draw_calls = 0;
		uint32_t quads = 0;
		uint32_t verticies = 0;
		uint32_t indicies = 0;
	};

	class Renderer2D{
		private:
			const uint32_t MAX_QUADS = 10000;
			const uint32_t MAX_VERTICIES = MAX_QUADS * 4;
			const uint32_t MAX_INDICIES = MAX_QUADS * 6;

		private:
			Ref<VertexArray> vertex_array;
			Ref<VertexBuffer> vertex_buffer;
			Ref<IndexBuffer> index_buffer;

			Ref<Shader> shader;

			uint32_t num_quad_indicies = 0;
			QuadVertex* quad_vertex_buffer = nullptr;
			QuadVertex* quad_vertex_buffer_ptr = nullptr;

			glm::vec4 quad_vertex_positions[4];

			Ref<OrthographicCamera> camera;


		public:
			Renderer2DPerformanceMetrics performanceMetrics;

			Renderer2D() = default;
			~Renderer2D() = default;


			void init(unsigned int width, unsigned int height);
			void begin_scene();
			void render();
			void end_scene();

			void set_projection(float width, float height){ camera->set_projection(width, height); }

		private:
			void reset();
			void draw();


			void clear_perf_metrics(){
				performanceMetrics.draw_calls = 0;
				performanceMetrics.quads = 0;
				performanceMetrics.verticies = 0;
				performanceMetrics.indicies = 0;
			}


		public:
			void draw_quad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color);
			void draw_quad(const glm::mat4& transform, const glm::vec4& color);
	};
	
}
