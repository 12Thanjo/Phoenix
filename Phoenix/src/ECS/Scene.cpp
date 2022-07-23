#include "ph_pch.h"
#include "Scene.h"

#include "components.h"
#include "Entity.h"
#include "Serializer.h"

#include "../rendering/Renderer2D.h"
#include "../rendering/Renderer3D.h"
#include "../render objects/cameras.h"
#include "src/scripting/scripting.h"
#include "src/Engine.h"



namespace Phoenix{

	Scene::Scene(const std::string& scene_name)
		: name(scene_name), uuid(){

		_serializer = new Serializer();

		PH_INFO("Initialized: ECS Scene");
	}


	Scene::Scene(const std::string& scene_name, const UUID& id)
		: name(scene_name), uuid(id){

		_serializer = new Serializer();

		PH_INFO("Initialized: ECS Scene");
	}



	Scene::~Scene(){
		delete _serializer;
	}


	//////////////////////////////////////////////////////////////////////
	// entities

	Entity Scene::createEntity(const std::string& name){
		performanceMetrics.entities += 1;

		Entity entity = {_registry.create(), this};
		entity.addComponent<Component::Name>(name);
		entity.addComponent<Component::UUID>();
		
		return entity;
	}

	Entity Scene::createEntity(const std::string& name, const UUID& uuid){
		performanceMetrics.entities += 1;

		Entity entity = {_registry.create(), this};
		entity.addComponent<Component::Name>(name);
		entity.addComponent<Component::UUID>(uuid);
		
		return entity;
	}

	void Scene::destroyEntity(Entity entity){
		_registry.destroy(entity);
		performanceMetrics.entities -= 1;
	}


	void Scene::clear(){
		_registry.each([&](entt::entity entt_entity){
			_registry.destroy(entt_entity);
			performanceMetrics.entities -= 1;
		});
	}

	Entity Scene::getEntityByUUID(UUID uuid){
		Entity output;

		_registry.each([&](entt::entity entt_entity){
			Entity entity{entt_entity, this};
			if(entity.getComponent<Component::UUID>().id == uuid){
				output = entity;
			}
		});

		return output;
	}



	//////////////////////////////////////////////////////////////////////
	// systems

	void Scene::update(){
		_registry.view<Component::Transform>().each([&](entt::entity entity_id, auto& component){
			component.transform = glm::translate(glm::mat4(1.0f), component.position);

			component.transform = glm::rotate(component.transform, component.rotation.x, {1, 0, 0});
			component.transform = glm::rotate(component.transform, component.rotation.y, {0, 1, 0});
			component.transform = glm::rotate(component.transform, component.rotation.z, {0, 0, 1});

			component.transform = glm::scale(component.transform, component.scale);
		});



		auto perspective_camera_group = _registry.group<Component::PerspectiveCamera>(entt::get<Component::Transform>);
		for(auto entt_entity : perspective_camera_group){
			auto [camera, transform] = perspective_camera_group.get<Component::PerspectiveCamera, Component::Transform>(entt_entity);

			camera.camera.setPosition(transform.position);
			camera.camera.setRotation(transform.rotation);	
		}


	}

	void Scene::render2D(Renderer2D* renderer_2d, Camera& camera)
	{
		auto sprite_group = _registry.group<Component::SpriteRenderer>(entt::get<Component::Transform>);
		for(auto entt_entity : sprite_group){
			auto [transform, sprite] = sprite_group.get<Component::Transform, Component::SpriteRenderer>(entt_entity);
			
			renderer_2d->drawQuad(transform.transform, sprite.color, camera);
		}
		
	}

	void Scene::render3D(Renderer3D* renderer_3d, Camera& camera){

		auto cube_group = _registry.group<Component::Cube>(entt::get<Component::Transform>);
		for(auto entt_entity : cube_group){
			auto [transform, cube] = cube_group.get<Component::Transform, Component::Cube>(entt_entity);
			
			renderer_3d->drawCube((uint32_t)entt_entity, transform.transform, cube.material, static_cast<PerspectiveCamera&>(camera), sunlight);
		}


		auto plane_group = _registry.group<Component::Plane>(entt::get<Component::Transform>);
		for(auto entt_entity : plane_group){
			auto [transform, plane] = plane_group.get<Component::Transform, Component::Plane>(entt_entity);
			
			renderer_3d->drawPlane((uint32_t)entt_entity, transform.transform, plane.material, static_cast<PerspectiveCamera&>(camera), sunlight);
		}

	}


	void Scene::runCreateScripts(Scripting& scripting, Engine* engine){
		_registry.view<Component::Script>().each([&](entt::entity entity_id, auto& component){
			Entity entity {entity_id, this};
			scripting.scriptControllerCreate(component.path, entity, engine);
		});
	}

	void Scene::runScripts(Scripting& scripting, Engine* engine){
		scripting.updateInit(engine);

		_registry.view<Component::Script>().each([&](entt::entity entity_id, auto& component){
			Entity entity {entity_id, this};
			scripting.scriptControllerUpdate(component.path, entity, engine);
		});
	}


	//////////////////////////////////////////////////////////////////////
	// misc

	void Scene::forEach(std::function<void(Entity)> func){
		_registry.each([&](entt::entity entt_entity){
			Entity entity{entt_entity, this};
			func(entity);
		});
	}



	void Scene::serialize(const std::string& filepath){
		_serializer->serialize(this, filepath);
	}

	std::string Scene::deserialize(const std::string& filepath, Scripting& scripting){
		return _serializer->deserialize(this, filepath, scripting);
	}




	//////////////////////////////////////////////////////////////////////
	// startup camera


	UUID Scene::getStartupCamera() const {
		PH_ASSERT(_has_startup_camera, "Startup Camera has not been set");
		return _startup_camera;
	}

	void Scene::setStartupCamera(UUID camera){
		_startup_camera = camera;
		_has_startup_camera = true;
	}

}
