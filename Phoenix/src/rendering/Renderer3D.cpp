#include "ph_pch.h"
#include "Renderer3D.h"

#include "buffers.h"

#include "src/assets/AssetManager.h"
#include "src/assets/materials.h"

#include "src/render objects/cameras.h"
#include "src/render objects/lights.h"


namespace Phoenix{
	
	Renderer3D::Renderer3D(AssetManager* asset_manager)
		: _asset_manager(asset_manager) {
	}

	void Renderer3D::init(){
		_basic_shader = _asset_manager->loadShader("./assets/shaders/basic3d.shader");
		_card_shader = _asset_manager->loadShader("./assets/shaders/card.shader");
	};



	void Renderer3D::bindBasic(PerspectiveCamera& camera, Lights::Directional& sunlight){
		_asset_manager->bindShader(_basic_shader);
		sunlight.upload(_asset_manager, _basic_shader);

		_asset_manager->uploadMat4(_basic_shader, "u_view_projection", camera.getViewProjection());
		_asset_manager->uploadFloat3(_basic_shader, "u_camera_position", camera.getPosition());

		_light_index = 0;
	}

	void Renderer3D::bindPointLight(Lights::Point& light, glm::vec3& position){
		if(_light_index < 16){
			light.upload(_asset_manager, _basic_shader, position, _light_index);
			_light_index += 1;
		}else{
			PH_WARNING("Maximum Point lights is 16");
		}
	}

	void Renderer3D::finishBindBasic(){
		_asset_manager->uploadInt(_basic_shader, "u_num_point_lights", _light_index);
	}


	void Renderer3D::bindCard(PerspectiveCamera& camera){
		_asset_manager->bindShader(_card_shader);

		_asset_manager->uploadMat4(_card_shader, "u_view_projection", camera.getViewProjection());
	}



	void Renderer3D::basicMaterialUpload(uint32_t entt_id, glm::mat4& transform, BasicMaterial& material){
		_asset_manager->uploadMat4(_basic_shader, "u_model", transform);
		
		if(material.usingTexture()){
			if(_asset_manager->hasTexture(material.getTexture())){
				// use texture
				_asset_manager->bindTexture(material.getTexture());
				_asset_manager->uploadInt(_basic_shader, "u_texture", 3);
				_asset_manager->uploadInt(_basic_shader, "u_using_texture", 1);
				_asset_manager->uploadFloat4(_basic_shader, "u_color", material.color);
			}else{
				// texture not found
				_asset_manager->uploadInt(_basic_shader, "u_using_texture", 0);
				_asset_manager->uploadFloat4(_basic_shader, "u_color", {1, 0, 1, 1});
			}
		}else{
			// just color
			_asset_manager->uploadInt(_basic_shader, "u_using_texture", 0);
			_asset_manager->uploadFloat4(_basic_shader, "u_color", material.color);
		}

		_asset_manager->uploadInt(_basic_shader, "u_entity_id", entt_id);


		_asset_manager->uploadFloat(_basic_shader, "u_shininess", material.shininess);
	}


	void Renderer3D::drawCube(uint32_t entt_id, glm::mat4& transform, BasicMaterial& material){
		
		basicMaterialUpload(entt_id, transform, material);

		// vertex data, buffers, and attributes
		static float verticies[] = {
			// positions				//text coords	// normals
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



			 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,      0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,        1.0f, 0.0f,      0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,      0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,        0.0f, 0.0f,      0.0f,  0.0f,  1.0f,

			 0.5f,  0.5f, -0.5f,		0.0f, 1.0f,      0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,        0.0f, 0.0f,      0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,      0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,        1.0f, 0.0f,      0.0f,  0.0f, -1.0f,

		};

		static uint32_t indicies[] = {
			 0,  1,  2,		 0,  2,  3,
			 4,  6,  5,		 4,  7,  6,

			 8, 10,  9,		 9, 10, 11,
			12, 13, 14,		13, 15, 14,

			16, 17, 18,		17, 19, 18,
			20, 22, 21,		21, 22, 23
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
		performanceMetrics.verticies += 8;
		performanceMetrics.indicies  += 36;
	}



	void Renderer3D::drawPlane(uint32_t entt_id, glm::mat4& transform, BasicMaterial& material){
		basicMaterialUpload(entt_id, transform, material);


		// vertex data, buffers, and attributes
		static float verticies[] = {
			-0.5f,  0.0f, -0.5f,		0.0f, 1.0f,     0.0f,  1.0f,  0.0f,
			 0.5f,  0.0f, -0.5f,		1.0f, 1.0f,     0.0f,  1.0f,  0.0f,
			 0.5f,  0.0f,  0.5f,		1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
			-0.5f,  0.0f,  0.5f,		0.0f, 0.0f,     0.0f,  1.0f,  0.0f,
		};

		static uint32_t indicies[] = {
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



	void Renderer3D::drawCard(uint32_t entt_id, glm::mat4& transform, UUID& texture){
		_asset_manager->uploadMat4(_card_shader, "u_model", transform);

		if(_asset_manager->hasTexture(texture)){
			// use texture
			_asset_manager->bindTexture(texture);
			_asset_manager->uploadInt(_card_shader, "u_texture", 3);
			_asset_manager->uploadInt(_card_shader, "u_using_texture", 1);
			_asset_manager->uploadFloat4(_card_shader, "u_color", {1, 1, 1, 1});
		}else{
			// texture not found
			_asset_manager->uploadInt(_card_shader, "u_using_texture", 0);
			_asset_manager->uploadFloat4(_card_shader, "u_color", {1, 0, 1, 1});
		}

		_asset_manager->uploadInt(_card_shader, "u_entity_id", entt_id);



		//////////////////////////////////////////////////////////////////////////////////////////////////////


		// vertex data, buffers, and attributes
		static float verticies[] = {
			-0.5f,  0.0f, -0.5f,		0.0f, 1.0f,
			 0.5f,  0.0f, -0.5f,		1.0f, 1.0f,
			 0.5f,  0.0f,  0.5f,		1.0f, 0.0f,
			-0.5f,  0.0f,  0.5f,		0.0f, 0.0f,
		};

		static uint32_t indicies[] = {
			0,  1,  2,		 0,  2,  3,
		};


		// create buffers
		VertexArray va{};

		VertexBuffer vb(verticies, sizeof(verticies));
		vb.layout<float>(3); // positions
		vb.layout<float>(2); // texture coordinates
		va.addVertexBuffer(vb);


		IndexBuffer index_buffer(indicies, sizeof(indicies));
		// va.addIndexBuffer(index_buffer);

		va.bind();
		glDrawElements(GL_TRIANGLES, sizeof(indicies), GL_UNSIGNED_INT, 0);
		va.unbind();


	}



	void Renderer3D::resetPerfMetrics(){
		performanceMetrics.drawCalls = 0;
		performanceMetrics.verticies = 0;
		performanceMetrics.indicies  = 0;
	}

}
