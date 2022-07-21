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
			void updateInit(Engine* engine); //run this once every frame before running update scripts
			void scriptControllerUpdate(std::string script_id, Entity entity, Engine* engine);

			//////////////////////////////////////////////////////////////////////
			// transform
			double getX(uint32_t id);
			void setX(uint32_t id, double x);

			double getY(uint32_t id);
			void setY(uint32_t id, double y);

			double getZ(uint32_t id);
			void setZ(uint32_t id, double z);


			//////////////////////////////////////////////////////////////////////
			// inputs
			bool keyDown(int key);


			//////////////////////////////////////////////////////////////////////
			// engine
			inline double frameTime() const { return _frame_time; };

		private:
			Engine* _engine;
			double _frame_time;

	};

	
}
