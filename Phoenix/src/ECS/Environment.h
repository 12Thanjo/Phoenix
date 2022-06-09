#pragma once

#include <lib/EnTT/entt.hpp>
#include "Serializer.h"


namespace Phoenix{

	class Entity;
	class Renderer2D;
	class Camera;
	class UUID;


	struct EnvironmentPerformanceMetrics{
		unsigned int entities = 0;
	};
	
	
	class Environment{
		public:
			Environment();
			~Environment();
			
			Entity createEntity(const std::string& name = "Entity");
			Entity createEntity(const std::string& name, const UUID& uuid);
			void destroyEntity(Entity entity);
			void clear();


			void update();
			void render(Renderer2D* renderer_2d, Camera& camera);



			template<typename T>
			void each(std::function<void(Entity, T&)> func){
				_registry.view<T>().each([&](entt::entity entity_id, auto& component){
					func(Entity{entity_id, this}, component);
				});
			}

			void forEach(std::function<void(Entity)> func);


			void serialize(const std::string& filepath);
			void deserialize(const std::string& filepath);
		

		public:
			EnvironmentPerformanceMetrics performanceMetrics;

		private:
			entt::registry _registry;
			Serializer* _serializer;

		private:
			inline bool _valid_entity(entt::entity entt_entity){ return _registry.valid(entt_entity); }


		friend class Entity;
	};

}
