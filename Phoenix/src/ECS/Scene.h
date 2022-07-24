#pragma once

#define ENTT_ENABLE_ETO
#include <lib/EnTT/entt.hpp>
#include "Serializer.h"
#include "src/render objects/cameras.h"
#include "src/render objects/lights.h"

namespace Phoenix{

	class Entity;
	class Renderer2D;
	class Renderer3D;
	class Camera;
	class UUID;
	class Scripting;
	class Engine;


	struct ScenePerformanceMetrics{
		unsigned int entities = 0;
		float update = 0;
	};
	
	
	class Scene{
		public:
			Scene(const std::string& scene_name = "Unnamed");
			Scene(const std::string& scene_name, const UUID& id);
			~Scene();
			
			//////////////////////////////////////////////////////////////////////
			// entities
			Entity createEntity(const std::string& name = "Entity");
			Entity createEntity(const std::string& name, const UUID& uuid);
			void destroyEntity(Entity entity);
			void clear();
			Entity getEntityByUUID(UUID uuid);

			//////////////////////////////////////////////////////////////////////
			// systems
			void update();
			void render2D(Renderer2D* renderer_2d, Camera& camera);
			void render3D(Renderer3D* renderer_3d, Camera& camera);
			void runCreateScripts(Scripting& scripting, Engine* engine);
			void runScripts(Scripting& scripting, Engine* engine);



			template<typename T>
			void each(std::function<void(Entity, T&)> func){
				_registry.view<T>().each([&](entt::entity entity_id, auto& component){
					func(Entity{entity_id, this}, component);
				});
			}

			void forEach(std::function<void(Entity)> func);


			void serialize(const std::string& filepath);
			std::string deserialize(const std::string& filepath, Scripting& scripting);


			inline bool hasStartupCamera() const { return _has_startup_camera; };
			UUID getStartupCamera() const;
			void setStartupCamera(UUID camera);

			void resetPerfMetrics();
		public:
			ScenePerformanceMetrics performanceMetrics;
			std::string name;
			UUID uuid;

			OrbitalCamera camera{};
			Lights::Directional sunlight{};


		private:
			inline bool _valid_entity(entt::entity entt_entity){ return _registry.valid(entt_entity); }


		private:
			entt::registry _registry;
			Serializer* _serializer;
			
			UUID _startup_camera;
			bool _has_startup_camera = false;

		


		friend class Entity;
	};

}
