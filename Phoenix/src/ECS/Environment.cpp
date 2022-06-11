#include "ph_pch.h"
#include "Environment.h"

#include "components.h"
#include "Entity.h"
#include "Serializer.h"

#include "../rendering/Renderer2D.h"
#include "../rendering/Renderer3D.h"
#include "../render objects/cameras.h"



namespace Phoenix{

	Environment::Environment(){
		_serializer = new Serializer();


		PH_INFO("Initialized: ECS Environment");
	}



	Environment::~Environment(){
		delete _serializer;
	}





	Entity Environment::createEntity(const std::string& name){
		performanceMetrics.entities += 1;

		Entity entity = {_registry.create(), this};
		entity.addComponent<Component::Name>(name);
		entity.addComponent<Component::UUID>();
		
		return entity;
	}

	Entity Environment::createEntity(const std::string& name, const UUID& uuid){
		performanceMetrics.entities += 1;

		Entity entity = {_registry.create(), this};
		entity.addComponent<Component::Name>(name);
		entity.addComponent<Component::UUID>(uuid);
		
		return entity;
	}

	void Environment::destroyEntity(Entity entity){
		_registry.destroy(entity);
		performanceMetrics.entities -= 1;
	}


	void Environment::clear(){
		_registry.each([&](entt::entity entt_entity){
			_registry.destroy(entt_entity);
			performanceMetrics.entities -= 1;
		});
	}



	void Environment::update(){
		_registry.view<Component::Transform>().each([&](entt::entity entity_id, auto& tc){
			tc.transform = glm::translate(glm::mat4(1.0f), tc.translation);

			tc.transform = glm::rotate(tc.transform, tc.rotation.x, {1, 0, 0});
			tc.transform = glm::rotate(tc.transform, tc.rotation.y, {0, 1, 0});
			tc.transform = glm::rotate(tc.transform, tc.rotation.z, {0, 0, 1});

			tc.transform = glm::scale(tc.transform, tc.scale);
		});



		auto camera_group = _registry.group<Component::Camera>(entt::get<Component::Transform>);
		for(auto entt_entity : camera_group){
			auto [camera, transform] = camera_group.get<Component::Camera, Component::Transform>(entt_entity);

			camera.camera.setPosition(transform.translation);
			camera.camera.setRotation(transform.rotation);
				
		}

	}

	void Environment::render2D(Renderer2D* renderer_2d, Camera& camera)
	{
		auto sprite_group = _registry.group<Component::SpriteRenderer>(entt::get<Component::Transform>);
		for(auto entt_entity : sprite_group){
			auto [transform, sprite] = sprite_group.get<Component::Transform, Component::SpriteRenderer>(entt_entity);
			
			renderer_2d->drawQuad(transform.transform, sprite.color, camera);
		}
		
	}

	void Environment::render3D(Renderer3D* renderer_3d, Camera& camera){

		auto cube_group = _registry.group<Component::Cube>(entt::get<Component::Transform>);
		for(auto entt_entity : cube_group){
			auto [transform, cube] = cube_group.get<Component::Transform, Component::Cube>(entt_entity);
			
			renderer_3d->drawCube(transform.transform, cube.color, camera);
		}

	}



	void Environment::forEach(std::function<void(Entity)> func){
		_registry.each([&](entt::entity entt_entity){
			Entity entity{entt_entity, this};
			func(entity);
		});
	}





	void Environment::serialize(const std::string& filepath){
		_serializer->serialize(this, filepath);
	}

	void Environment::deserialize(const std::string& filepath){
		_serializer->deserialize(this, filepath);
	}

}


#undef SINGLE