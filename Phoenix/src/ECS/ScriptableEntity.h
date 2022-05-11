#pragma once

#include "Entity.h"

namespace Phoenix{
	
	class ScriptableEntity{
		friend class Environment;

		private:
			Entity entity;
			
		public:
			virtual ~ScriptableEntity(){};


			template<typename T>
			T& getComponent(){
				return entity.getComponent<T>();
			};


		protected:
			virtual void create(){};
			virtual void destroy(){};
			virtual void update(){};
			
	};

}
