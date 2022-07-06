#include "ph_pch.h"
#include "Renderer3D.h"

#include "buffers.h"
#include "src/assets/AssetManager.h"
#include "src/render objects/cameras.h"
#include "src/render objects/lights.h"


namespace Phoenix{
	
	Renderer3D::Renderer3D(AssetManager* asset_manager)
		: _asset_manager(asset_manager) {
	}

	void Renderer3D::init(){
		_basic_shader = _asset_manager->loadShader("./assets/shaders/working.shader");
	};


	void Renderer3D::drawCube(glm::mat4& transform, glm::vec4& color, PerspectiveCamera& camera, Lights::Directional& sunlight){
		_asset_manager->bindShader(_basic_shader);



		_asset_manager->uploadMat4(_basic_shader, "u_model", transform);
		_asset_manager->uploadMat4(_basic_shader, "u_view_projection", camera.getViewProjection());
		_asset_manager->uploadFloat4(_basic_shader, "u_color", color);
		_asset_manager->uploadFloat3(_basic_shader, "u_camera_position", camera.getPosition());

		sunlight.upload(_asset_manager, _basic_shader);



		_asset_manager->uploadFloat(_basic_shader, "u_shininess", 32);


		// vertex data, buffers, and attributes
		float verticies[] = {
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,     0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,     0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,     0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,        0.0f, 0.0f,     0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,        1.0f, 0.0f,     0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,        1.0f, 1.0f,     0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,        0.0f, 1.0f,     0.0f, -1.0f,  0.0f,


			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,     1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,		0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,        1.0f, 0.0f,     1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,        0.0f, 0.0f,     1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,    -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,		1.0f, 1.0f,    -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,        0.0f, 0.0f,    -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,        1.0f, 0.0f,    -1.0f,  0.0f,  0.0f,



			 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,        1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,        0.0f, 0.0f,    0.0f,  0.0f,  1.0f,

			 0.5f,  0.5f, -0.5f,		0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,        0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,        1.0f, 0.0f,    0.0f,  0.0f, -1.0f,

		};

		uint32_t indicies[] = {
			 0,  1,  2,		 0,  2,  3,
			 4,  5,  6,		 4,  6,  7,

			 8,  9, 10,		 9, 10, 11,
			12, 13, 14,		13, 14, 15,

			16, 17, 18,		17, 18, 19,
			20, 21, 22,		21, 22, 23
		};


		// create buffers
		VertexArray va{};

		VertexBuffer vb(verticies, sizeof(verticies));
		vb.layout<float>(3); // positions
		vb.layout<float>(2);	// texture coordinates
		vb.layout<float>(3); // normals
		va.addVertexBuffer(vb);


		IndexBuffer index_buffer(indicies, sizeof(indicies));
		// va.addIndexBuffer(index_buffer);



		va.bind();
		glDrawElements(GL_TRIANGLES, sizeof(indicies), GL_UNSIGNED_INT, 0);
		va.unbind();



		performanceMetrics.drawCalls += 1;
		performanceMetrics.verticies += 8;
		performanceMetrics.indicies  += 36;
	}



	void Renderer3D::drawPlane(glm::mat4& transform, glm::vec4& color, PerspectiveCamera& camera, Lights::Directional& sunlight){
		_asset_manager->bindShader(_basic_shader);



		_asset_manager->uploadMat4(_basic_shader, "u_model", transform);
		_asset_manager->uploadMat4(_basic_shader, "u_view_projection", camera.getViewProjection());
		_asset_manager->uploadFloat4(_basic_shader, "u_color", color);
		_asset_manager->uploadFloat3(_basic_shader, "u_camera_position", camera.getPosition());

		sunlight.upload(_asset_manager, _basic_shader);



		_asset_manager->uploadFloat(_basic_shader, "u_shininess", 32);


		// vertex data, buffers, and attributes
		float verticies[] = {
			-0.5f,  0.0f, -0.5f,		0.0f, 1.0f,     0.0f,  1.0f,  0.0f,
			 0.5f,  0.0f, -0.5f,		1.0f, 1.0f,     0.0f,  1.0f,  0.0f,
			 0.5f,  0.0f,  0.5f,		1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
			-0.5f,  0.0f,  0.5f,		0.0f, 0.0f,     0.0f,  1.0f,  0.0f,
		};

		uint32_t indicies[] = {
			0,  1,  2,		 0,  2,  3,
		};


		// create buffers
		VertexArray va{};

		VertexBuffer vb(verticies, sizeof(verticies));
		vb.layout<float>(3); // positions
		vb.layout<float>(2); // texture coordinates
		vb.layout<float>(3); // normals
		va.addVertexBuffer(vb);


		IndexBuffer index_buffer(indicies, sizeof(indicies));
		// va.addIndexBuffer(index_buffer);

		va.bind();
		glDrawElements(GL_TRIANGLES, sizeof(indicies), GL_UNSIGNED_INT, 0);
		va.unbind();



		performanceMetrics.drawCalls += 1;
		performanceMetrics.verticies += 4;
		performanceMetrics.indicies  += 6;
	}



	void Renderer3D::resetPerfMetrics(){
		performanceMetrics.drawCalls = 0;
		performanceMetrics.verticies = 0;
		performanceMetrics.indicies  = 0;
	}

}
