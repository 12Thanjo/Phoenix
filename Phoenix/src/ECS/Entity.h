#pragma once


// #include <lib/EnTT/entt.hpp>
#include "Environment.h"


namespace Phoenix{

	class Entity{
		private:
			entt::entity _id{ entt::null };
			Environment* _environment = nullptr;
	
		public:
			Entity() = default;
			Entity(entt::entity id, Environment* environment) : _id(id), _environment(environment) {};
			~Entity(){};


			template<typename T, typename... Args>
			T& addComponent(Args&&... args){
				PH_ASSERT(!hasComponent<T>(), "attempted to add component to an Entity that already has it");
				return _environment->_registry.emplace<T>(_id, std::forward<Args>(args)...);
			}


			template<typename T>
			T& getComponent(){
				PH_ASSERT(hasComponent<T>(), "attempted to get the component of an Entity that doesn't have it");
				return _environment->_registry.get<T>(_id);
			}

			template<typename T>
			bool hasComponent(){
				return _environment->_registry.all_of<T>(_id);
			}


			template<typename T>
			void removeComponent(){
				PH_ASSERT(hasComponent<T>(), "attempted to remove a component from an Entity that doesn't have it");
				_environment->_registry.remove<T>(_id);
			}



			operator bool() const { return (_id != entt::null) && (_environment->_valid_entity(_id)); };
			operator entt::entity() const { return _id; };
			operator uint32_t() const { return (uint32_t)_id; };
			bool operator ==(const Entity& other) const {
				return _id == other._id && _environment == other._environment;
			};
			bool operator !=(const Entity& other) const {
				return !(*this == other);
			};
	};
	
}


