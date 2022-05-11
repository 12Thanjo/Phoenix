#pragma once


#include "Environment.h"
#include <lib/EnTT/entt.hpp>


namespace Phoenix{

	class Entity{
		private:
			entt::entity id{ entt::null };
			Environment* environment = nullptr;
	
		public:
			Entity() = default;
			Entity(entt::entity _id, Environment* _environment)	: id(_id), environment(_environment) {};
			Entity(const Entity& other) = default;

			template<typename T, typename... Args>
			T& addComponent(Args&&... args){
				PH_ASSERT(!hasComponent<T>(), "attempted to add component to an Entity that already has it");
				return environment->registry.emplace<T>(id, std::forward<Args>(args)...);
			}


			template<typename T>
			T& getComponent(){
				PH_ASSERT(hasComponent<T>(), "attempted to get the component of an Entity that doesn't have it");
				return environment->registry.get<T>(id);
			}

			template<typename T>
			bool hasComponent(){
				return environment->registry.all_of<T>(id);
			}


			template<typename T>
			void removeComponent(){
				PH_ASSERT(hasComponent<T>(), "attempted to remove a component from an Entity that doesn't have it");
				environment->registry.remove<T>(id);
			}



			operator bool() const { return id != entt::null; };
			operator entt::entity() const { return id; };
			operator uint32_t() const { return (uint32_t)id; };
			bool operator ==(const Entity& other) const {
				return id == other.id && environment == other.environment;
			};
			bool operator !=(const Entity& other) const {
				return !(*this == other);
			};
	
	};

}
