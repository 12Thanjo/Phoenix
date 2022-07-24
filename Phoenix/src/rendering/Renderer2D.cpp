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

	
	void Renderer2D::drawQuad(uint32_t entt_id, glm::mat4& transform, glm::vec4& color, Camera& camera){
		_asset_manager->bindShader(_quad_shader);

		_asset_manager->uploadMat4(_quad_shader, "u_model", transform);
		// _asset_manager->uploadMat4(_quad_shader, "u_view_projection", camera.getViewProjection());
		_asset_manager->uploadFloat4(_quad_shader, "u_color", color);

		_asset_manager->uploadInt(_quad_shader, "u_entity_id", entt_id);

		
		float verticies[] = {
			-0.5f,  0.5f,
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
		};


		uint32_t indicies[]{
			0, 1, 2,
			0, 2, 3
		};


		VertexArray va{};

		VertexBuffer vb(verticies, sizeof(verticies));
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


	void Renderer2D::resetPerfMetrics(){
		performanceMetrics.drawCalls = 0;
		performanceMetrics.verticies = 0;
		performanceMetrics.indicies  = 0;
	}

}
