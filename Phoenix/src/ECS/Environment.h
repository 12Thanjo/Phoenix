#pragma once

#include <lib/EnTT/entt.hpp>

// #include "components.h"
// #include "Entity.h"

#include "src/rendering/renderers/Renderer.h"

namespace Phoenix{
	struct EnvironmentPerformanceMetrics{
		uint32_t entities = 0;
	};

	class Entity;

	class Environment{
		private:
			entt::registry registry;
	
		public:
			EnvironmentPerformanceMetrics performanceMetrics;


			Environment();
			~Environment();

			Entity create_entity(const std::string& name = "Entity");
			void destroy_entity(Entity entity);

			void update(Ref<Renderer> renderer);


			entt::registry& _get_registry() { return registry; };

		friend class Entity;
			
	};

}
