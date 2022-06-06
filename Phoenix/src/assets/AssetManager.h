#pragma once

#include "core/UUID.h"

namespace Phoenix{
	class Shader;


	class AssetManager{
		private:
			std::unordered_map<UUID, Shader*> _shaders;
			std::unordered_map<UUID, UUID*> _uuids;
			std::map<std::string, int> test;
	
		public:
			AssetManager() = default;
			~AssetManager();


			UUID loadShader(std::string filepath);
			void bindShader(UUID& id);


			void uploadFloat(UUID& id, const std::string& name, const float& value);
			void uploadFloat2(UUID& id, const std::string& name, const glm::vec2& values);
			void uploadFloat3(UUID& id, const std::string& name, const glm::vec3& values);
			void uploadFloat4(UUID& id, const std::string& name, const glm::vec4& values);


			void uploadMat4(UUID& id, const std::string& name, const glm::mat4& matrix);
			void uploadInt(UUID& id, const std::string& name, const int& value);

	};

}
