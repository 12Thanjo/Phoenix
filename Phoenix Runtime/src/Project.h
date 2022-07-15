#pragma once

#include <Phoenix.h>

namespace Phoenix{
	class Engine;
	
	class Project{
		public:
			Project() = default;
			~Project() = default;

			void serialize();
			std::string deserialize(std::string filepath, Engine* engine);
			std::string deserialize(std::filesystem::path filepath, Engine* engine);

			void setStartupScene(UUID startup_scene);
			UUID getStartupScene() const;
			inline bool hasStartupScene() const { return _has_startup_scene; };

			std::string getRelativePath();

		public:
			std::filesystem::path path;

			// assets
			Utils::Bimap<std::string, UUID> scenes;
			Utils::Bimap<std::string, UUID> scripts;
			Utils::Bimap<std::string, UUID> textures;

		private:
			std::string deserialize_functionality(Engine* engine);
			bool index();

		private:
			UUID _startup_scene;
			bool _has_startup_scene = false;

	};

}
