#pragma once

#include <Phoenix.h>

namespace Phoenix{
	
	class Project{
		public:
			Project() = default;
			~Project() = default;

			void serialize();
			std::string deserialize(std::string filepath);
			std::string deserialize(std::filesystem::path filepath);

			void setStartupScene(UUID startup_scene);
			UUID getStartupScene() const;
			inline bool hasStartupScene() const { return _has_startup_scene; };

			std::string getRelativePath();

		public:
			std::filesystem::path path;

			// assets
			Utils::Bimap<std::string, UUID> scenes;
			Utils::Bimap<std::string, UUID> scripts;

		private:
			std::string deserialize_functionality();
			bool index();

		private:
			UUID _startup_scene;
			bool _has_startup_scene = false;

	};

}
