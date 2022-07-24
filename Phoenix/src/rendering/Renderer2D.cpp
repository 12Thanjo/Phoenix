#include "ph_pch.h"
#include "Renderer2D.h"

#include "buffers.h"
#include "../assets/AssetManager.h"
#include "../render objects/cameras.h"


namespace Phoenix{
	Renderer2D::Renderer2D(AssetManager* asset_manager)
		: _asset_manager(asset_manager) {
	}


	void Renderer2D::init(){
		_quad_shader = _asset_manager->loadShader("./assets/shaders/basic2d.shader");
	}

	void Renderer2D::bind(){
		_asset_manager->bindShader(_quad_shader);
	}


	void Renderer2D::draw_quad(uint32_t entt_id, glm::mat4& transform, glm::vec4& color){
		_asset_manager->uploadMat4(_quad_shader, "u_model", transform);
		_asset_manager->uploadFloat4(_quad_shader, "u_color", color);

		_asset_manager->uploadInt(_quad_shader, "u_entity_id", entt_id);

		
		static float verticies[] = {
			// positions	 //text coords
			-0.5f,  0.5f,	 0.0f,  1.0f,
			-0.5f, -0.5f,	 0.0f,  0.0f,
			 0.5f, -0.5f,	 1.0f,  0.0f,
			 0.5f,  0.5f,	 1.0f,  1.0f,
		};


		static uint32_t indicies[] = {
			0, 1, 2,
			0, 2, 3
		};


		VertexArray va{};

		VertexBuffer vb(verticies, sizeof(verticies));
		vb.layout<float>(2);
		vb.layout<float>(2);
		va.addVertexBuffer(vb);

		IndexBuffer ib(indicies, sizeof(indicies));

		va.bind();
		glDrawElements(GL_TRIANGLES, sizeof(indicies), GL_UNSIGNED_INT, 0);
		va.unbind();


		performanceMetrics.drawCalls += 1;
		performanceMetrics.verticies += 4;
		performanceMetrics.indicies  += 6;
	}

	
	void Renderer2D::drawQuad(uint32_t entt_id, glm::mat4& transform, glm::vec4& color){
		_asset_manager->uploadInt(_quad_shader, "u_using_texture", 0);
		draw_quad(entt_id, transform, color);
	}


	void Renderer2D::drawQuad(uint32_t entt_id, glm::mat4& transform, glm::vec4& color, UUID& texture){
		glm::vec4 color_output = color;

		if(_asset_manager->hasTexture(texture)){
			// use texture
			_asset_manager->bindTexture(texture);
			_asset_manager->uploadInt(_quad_shader, "u_texture", 3);
			_asset_manager->uploadInt(_quad_shader, "u_using_texture", 1);
		}else{
			// texture not found
			_asset_manager->uploadInt(_quad_shader, "u_using_texture", 0);
			color_output = {1, 0, 1, 1};
		}

		draw_quad(entt_id, transform, color_output);
	}


	void Renderer2D::resetPerfMetrics(){
		performanceMetrics.drawCalls = 0;
		performanceMetrics.verticies = 0;
		performanceMetrics.indicies  = 0;
	}

}
