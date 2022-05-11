#include "ph_pch.h"
#include "Renderer2D.h"



namespace Phoenix{

	void Renderer2D::init(unsigned int width, unsigned int height){
		camera = CreateRef<OrthographicCamera>(width, height);
		// camera->set_position({0.0f, 0.0f});
		// camera->set_rotation(0.0f);

		vertex_array = CreateRef<VertexArray>();

		vertex_buffer = CreateRef<VertexBuffer>(MAX_VERTICIES * sizeof(QuadVertex));
		vertex_buffer->push<float>(4);
		vertex_buffer->push<float>(4);

		vertex_array->add_vertex_buffer(vertex_buffer);


		quad_vertex_buffer = new QuadVertex[MAX_VERTICIES];


		Ref<uint32_t> quad_indicies(new uint32_t[MAX_INDICIES]);

		uint32_t offset = 0;
		for(uint32_t i = 0; i < MAX_INDICIES; i += 6){
			quad_indicies.get()[i + 0] = offset + 0;
			quad_indicies.get()[i + 1] = offset + 1;
			quad_indicies.get()[i + 2] = offset + 2;

			quad_indicies.get()[i + 3] = offset + 2;
			quad_indicies.get()[i + 4] = offset + 3;
			quad_indicies.get()[i + 5] = offset + 0;

			offset += 4;
		}


		index_buffer = CreateRef<IndexBuffer>(quad_indicies, MAX_INDICIES);
		vertex_array->set_index_buffer(index_buffer);


		quad_vertex_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		quad_vertex_positions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		quad_vertex_positions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		quad_vertex_positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		


		std::string vertex_source = R"(
			#version 450 core

			layout(location = 0) in vec4 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 view_projection_matrix;

			out vec4 t_color;

			void main(){
				t_color = a_color;
				gl_Position = view_projection_matrix * a_position;
				// gl_Position = vec4(a_position, 1.0);
			}

		)";


		std::string fragment_source = R"(
			#version 450 core

			layout(location = 0) out vec4 color;

			in vec4 t_color;

			void main(){
				color = t_color;
			}
		)";


		shader = CreateRef<Shader>(vertex_source, fragment_source);




		PH_INFO("Renderer2D Initialized");
	}





	void Renderer2D::begin_scene(){
		clear_perf_metrics();

		shader->bind();
		shader->upload_uniform_mat4("view_projection_matrix", camera->get_view_projection_matrix());

		reset();
	}

	void Renderer2D::render(){

		// for(int i = -300; i < 300; i += 15){
		// 	for(int j = -300; j < 300; j += 15){
		// 		draw_quad({i, j}, 0.0f, {5, 5}, { (float)((i + 300) / 600.0f), (float)((j + 300) / 600.0f), (float)((j + 300) / 600.0f), 1.0f});
		// 	}
		// }

	}

	void Renderer2D::end_scene(){
		draw();
	}

	void Renderer2D::draw(){
		uint32_t data_size = (uint8_t*)quad_vertex_buffer_ptr - (uint8_t*)quad_vertex_buffer;
		vertex_buffer->set_data(quad_vertex_buffer, data_size);
		
		glDrawElements(GL_TRIANGLES, num_quad_indicies, GL_UNSIGNED_INT, nullptr);

		performanceMetrics.draw_calls += 1;
	}

	void Renderer2D::reset(){
		num_quad_indicies = 0;
		quad_vertex_buffer_ptr = quad_vertex_buffer;
	}


	///////////////////////////////////////////////////////////////////


	void Renderer2D::draw_quad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color){
		if(num_quad_indicies >= MAX_QUADS){
			draw();
			reset();
		}


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), {position.x, position.y, 0.0f});
		transform = glm::rotate(transform, rotation, {0.0f, 0.0f, 1.0f});
		transform = glm::scale(transform, {scale.x, scale.y, 1.0f});

		draw_quad(transform, color);
	}

	void Renderer2D::draw_quad(const glm::mat4& transform, const glm::vec4& color){
		for(size_t i = 0; i < 4; i += 1){
			quad_vertex_buffer_ptr->position = transform * quad_vertex_positions[i];
			quad_vertex_buffer_ptr->color = color;

			quad_vertex_buffer_ptr += 1;
		}


		num_quad_indicies += 6;

		performanceMetrics.quads += 1;
		performanceMetrics.verticies += 4;
		performanceMetrics.indicies += 6;
	}
}
