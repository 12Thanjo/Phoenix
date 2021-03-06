#include "ph_pch.h"
#include "AssetManager.h"

#include "shaders.h"
#include "textures.h"
#include "src/utils/Files.h"

namespace Phoenix{

	AssetManager::~AssetManager(){
		for(std::unordered_map<UUID, Shader*>::iterator itr = _shaders.begin(); itr != _shaders.end(); itr++){
	       delete itr->second;
	   	}

	   	for(std::unordered_map<UUID, Texture*>::iterator itr = _textures.begin(); itr != _textures.end(); itr++){
	       delete itr->second;
	   	}
	}


	//////////////////////////////////////////////////////////////////////
	// shaders

	UUID AssetManager::loadShader(std::string filepath){
		UUID* uuid = new UUID();
		
		_shaders[*uuid] = new Shader(filepath);
		
		PH_LOG("Loaded and compiled Shader:"
			"\n\tfilepath: " << filepath << 
			"\n\tUUID:     " << uuid);

		return *uuid;
	}


	void AssetManager::bindShader(UUID& id){
		if(_shaders[id]){
			_shaders[id]->bind();
		}
	}



	void AssetManager::uploadFloat  (UUID& id, const std::string& name, const float& value){ 	  _shaders[id]->uploadFloat(name, value);   }; 
	void AssetManager::uploadFloat2 (UUID& id, const std::string& name, const glm::vec2& values){ _shaders[id]->uploadFloat2(name, values); }; 
	void AssetManager::uploadFloat3 (UUID& id, const std::string& name, const glm::vec3& values){ _shaders[id]->uploadFloat3(name, values); }; 
	void AssetManager::uploadFloat4 (UUID& id, const std::string& name, const glm::vec4& values){ _shaders[id]->uploadFloat4(name, values); }; 
	void AssetManager::uploadMat4   (UUID& id, const std::string& name, const glm::mat4& matrix){ _shaders[id]->uploadMat4(name, matrix);   }; 
	void AssetManager::uploadInt    (UUID& id, const std::string& name, const int& value){ 		  _shaders[id]->uploadInt(name, value);     };


	//////////////////////////////////////////////////////////////////////
	// textures

	UUID AssetManager::loadTexture(std::string filepath){
		UUID* uuid = new UUID();
		
		_textures[*uuid] = new Texture(filepath);
		
		PH_LOG("Loaded Texture:"
			"\n\tfilepath: " << filepath << 
			"\n\tUUID:     " << uuid);


		return *uuid;
	}

	void AssetManager::loadTexture(std::string filepath, UUID& uuid){
		_textures[uuid] = new Texture(filepath);
		
		PH_LOG("Loaded Texture:"
			"\n\tfilepath: " << filepath << 
			"\n\tUUID:     " << uuid);
	}

	void AssetManager::bindTexture(UUID& id){
		if(_textures[id]){
			_textures[id]->bind(3);
		}
	}

	bool AssetManager::hasTexture(UUID& id){
		return _textures[id];
	}

}
