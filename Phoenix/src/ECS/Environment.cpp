#include "ph_pch.h"
#include "Environment.h"

#include "components.h"
#include "Entity.h"
#include "Serializer.h"

#include "../rendering/Renderer2D.h"
#include "../render objects/cameras.h"



namespace Phoenix{

	Environment::Environment(){
		_serializer = new Serializer();


		PH_INFO("Initialized: ECS Environment");
	}



	Environment::~Environment(){
		free(_serializer);
	}





	Entity Environment::createEntity(const std::string& name){
		// performanceMetrics.entities += 1;

		Entity entity = {_registry.create(), this};
		entity.addComponent<Component::Name>(name);
		entity.addComponent<Component::UUID>();
		
		return entity;
	}

	void Environment::destroyEntity(Entity entity){
		_registry.destroy(entity);
		// performanceMetrics.entities -= 1;
	}


	void Environment::update(){
		_registry.view<Component::Transform>().each([&](entt::entity entity_id, auto& tc){
				tc.transform = glm::translate(glm::mat4(1.0f), tc.translation);

				// tc.transform = glm::rotate(tc.transform, tc.rotation.x, {1, 0, 0});
				// tc.transform = glm::rotate(tc.transform, tc.rotation.y, {0, 1, 0});
				tc.transform = glm::rotate(tc.transform, tc.rotation.z, {0, 0, 1});

				tc.transform = glm::scale(tc.transform, tc.scale);
		});



		auto camera_group = _registry.group<Component::Camera>(entt::get<Component::Transform>);
		for(auto entity : camera_group){
			auto [camera, transform] = camera_group.get<Component::Camera, Component::Transform>(entity);

			camera.camera.setPosition(transform.translation);
			camera.camera.setRotation(transform.rotation);
				
		}

	}

	void Environment::render(Renderer2D* renderer_2d, Camera& camera){


		auto sprite_group = _registry.group<Component::Transform>(entt::get<Component::SpriteRenderer>);
		for(auto entity : sprite_group){
			auto [transform, sprite] = sprite_group.get<Component::Transform, Component::SpriteRenderer>(entity);
			
			renderer_2d->drawQuad(transform.transform, sprite.color, camera);
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