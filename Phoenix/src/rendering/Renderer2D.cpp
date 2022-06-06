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
		_quad_shader = _asset_manager->loadShader("./assets/shaders/color.shader");
	}

	
	void Renderer2D::drawQuad(glm::mat4& transform, glm::vec4& color, Camera& camera){
		_asset_manager->bindShader(_quad_shader);

		_asset_manager->uploadMat4(_quad_shader, "u_model", transform);
		// _asset_manager->uploadMat4(_quad_shader, "u_view_projection", camera.getViewProjection());
		_asset_manager->uploadFloat4(_quad_shader, "u_color", color);

		
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
	}

}
