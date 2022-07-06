#pragma once

// #include <jxcore.h>
#include "src/ECS/entity.h"


namespace Phoenix{
	class Engine;


	class Scripting{
		public:
			Scripting() = default;
			~Scripting() = default;
	
			
			void init(std::string path);
			void close();

			void run(std::string code);


			void scriptControllerUpdate(std::string script_id, Entity entity, Engine* engine);

			//////////////////////////////////////////////////////////////////////
			// methods for running by JX

			void setId(std::string uuid);
			void restoreId();

			//////////////////////////////////////////////////////////////////////
			// transform
			double getX();
			void setX(double x);

			double getY();
			void setY(double y);

			double getZ();
			void setZ(double z);


			//////////////////////////////////////////////////////////////////////
			// inputs
			bool keyDown(int key);

		private:
			Entity _entity;
			Entity _entity_save;
			Engine* _engine;

	};

	
}
