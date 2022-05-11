#include "ph_pch.h"
#include "Environment.h"

#include "Entity.h"
#include "components.h"

namespace Phoenix{

	Environment::Environment(){

	}


	Environment::~Environment(){

	}

	Entity Environment::create_entity(const std::string& name){
		performanceMetrics.entities += 1;

		Entity entity = {registry.create(), this};
		entity.addComponent<Component::Name>(name);
		
		return entity;
	}

	void Environment::destroy_entity(Entity entity){
		registry.destroy(entity);
		performanceMetrics.entities -= 1;
	}


	void Environment::update(Ref<Renderer> renderer){

		registry.view<Component::NativeScript>().each([=](auto entity, auto& native_script_component){
			if(!native_script_component.instance){
				native_script_component.instance = native_script_component.instantiate_script();
				native_script_component.instance->entity = Entity{entity, this};
				native_script_component.instance->create();
			}

			native_script_component.instance->update();
		});


		registry.view<Component::Transform>().each([=](entt::entity entity_id, auto& tc){
				tc.transform = glm::translate(glm::mat4(1.0f), tc.translation);

				// tc.transform = glm::rotate(tc.transform, tc.rotation.x, {1, 0, 0});
				// tc.transform = glm::rotate(tc.transform, tc.rotation.y, {0, 1, 0});
				tc.transform = glm::rotate(tc.transform, tc.rotation.z, {0, 0, 1});

				tc.transform = glm::scale(tc.transform, tc.scale);
		});



		auto sprite_group = registry.group<Component::Transform>(entt::get<Component::SpriteRenderer>);
		for(auto entity : sprite_group){
			auto [transform, sprite] = sprite_group.get<Component::Transform, Component::SpriteRenderer>(entity);
			
			renderer->draw_quad(transform.transform, sprite.color);
		}

	}
	
}
