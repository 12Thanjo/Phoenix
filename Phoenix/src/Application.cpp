

#include "ph_pch.h"

#include "Application.h"
#include "CMD.h"
#include "InputManager.h"

#include "layers/layers/ImGuiLayer.h"

#include <glad/glad.h>



namespace Phoenix{

	#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::instance = nullptr;



	static GLenum ShaderDataType_to_OpenGL_base_type(ShaderDataType type){
		switch(type){
			case ShaderDataType::None    : 	return GL_FLOAT;
			case ShaderDataType::Float   : 	return GL_FLOAT;
			case ShaderDataType::Float2  : 	return GL_FLOAT;
			case ShaderDataType::Float3  : 	return GL_FLOAT;
			case ShaderDataType::Float4  : 	return GL_FLOAT;
			case ShaderDataType::Mat3    : 	return GL_FLOAT;
			case ShaderDataType::Mat4    : 	return GL_FLOAT;
			case ShaderDataType::Int     : 	return GL_INT;
			case ShaderDataType::Int2    : 	return GL_INT;
			case ShaderDataType::Int3    : 	return GL_INT;
			case ShaderDataType::Int4    : 	return GL_INT;
			case ShaderDataType::Bool    : 	return GL_BOOL;
		}

		ph_internal_error("Unknown ShaderDataType: " << (int)type);
		trace();
		return 0;
	}




	Application::Application(){
		ph_internal_assert(!instance, "Application already exists");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->set_callback(BIND_EVENT_FUNCTION(Application::onEvent));

		ImGui_layer = new ImGuiLayer();
		pushOverlay(ImGui_layer);

		////////////////////////////////////////////////
		
		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array); 

		float verticies[3 * 7] = {
			// position					// color
			-0.5f, -0.5f,  0.0f,		0.9f, 0.1f, 0.1f, 1.0f,
			 0.5f, -0.5f,  0.0f,		0.1f, 0.9f, 0.1f, 1.0f,
			 0.0f,  0.5f,  0.0f,		0.1f, 0.1f, 0.9f, 1.0f,
		};

		vertex_buffer.reset(VertexBuffer::create(verticies, sizeof(verticies)));
		// vertex_buffer.bind();

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "position" },
				{ ShaderDataType::Float4, "color" },
			};

			vertex_buffer->setLayout(layout);

			// delete layout
		}

		uint32_t index = 0;
		const auto& layout = vertex_buffer->getLayout();
		for(const auto& element : layout){
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.get_component_count(),
				ShaderDataType_to_OpenGL_base_type(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.get_stride(),
				(const void*)element.offset);
			index += 1;
		}



		unsigned int indicies[3] = {
			0, 1, 2
		};

		index_buffer.reset(IndexBuffer::create(indicies, sizeof(indicies) / sizeof(uint32_t) ));



		std::string vertex_src = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			out vec3 _position;
			out vec4 _color;

			void main(){
				_position = position;
				_color = color;
				gl_Position = vec4(position, 1.0);
			}
		)";


		std::string fragment_src = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 _position;
			in vec4 _color;

			void main(){
				// color = vec4(_position * 0.5 + 0.5, 1.0);
				color = _color;
			}
		)";


		shader.reset(new OpenGLShader(vertex_src, fragment_src));
	}

	Application::~Application(){

	}

	void Application::onEvent(Event& e){
		if(e.getType() == PH_WINDOW_CLOSE_EVENT){
			running = false;
		}

		InputManager::onEvent(e);


		for(Layer* layer : layers){
			layer->onEvent(e);
			if(e.handled){
				break;
			}
		}

	}


	void Application::run(){
		while(running){
			glClearColor(0, 0.06f, 0.13f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			shader->bind();
			glBindVertexArray(vertex_array);
			glDrawElements(GL_TRIANGLES, index_buffer->getCount(), GL_UNSIGNED_INT, nullptr);


			for(Layer* layer : layers){
				layer->run();
			}

			ImGui_layer->begin();
			for(Layer* layer : layers){
				layer->onImGuiRender();
			}
			ImGui_layer->end();

			window->run();
		}
	}



	void Application::pushLayer(Layer* layer){
		layers.push_layer(layer);
		layer->onAttatch();
	}

	void Application::pushOverlay(Layer* overlay){
		layers.push_overlay(overlay);
		overlay->onAttatch();
	}


	void Application::popLayer(Layer* layer){
		layers.pop_layer(layer);
		layer->onDetatch();
	}

	void Application::popOverlay(Layer* overlay){
		layers.pop_overlay(overlay);
		overlay->onDetatch();
	}


}


