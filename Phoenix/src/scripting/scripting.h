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

			void createScript(std::string script, std::string script_path);
			void scriptControllerCreate(std::string script_id, Entity entity, Engine* engine);
			void scriptControllerUpdate(std::string script_id, Entity entity, Engine* engine);

			//////////////////////////////////////////////////////////////////////
			// methods for running by JX

			// void setId(std::string uuid);
			// void restoreId();

			//////////////////////////////////////////////////////////////////////
			// transform
			double getX(uint32_t id);
			void setX(uint32_t id, double x);

			// double getY();
			// void setY(double y);

			// double getZ();
			// void setZ(double z);


			//////////////////////////////////////////////////////////////////////
			// inputs
			bool keyDown(int key);


			//////////////////////////////////////////////////////////////////////
			// engine
			double frameTime();

		private:
			Engine* _engine;

	};

	
}
