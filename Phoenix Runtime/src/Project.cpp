#include "Project.h"


#define DESERIALIZE_ERROR(msg) PH_WARNING(msg); \
	std::stringstream ss; \
	ss << msg; \
	return ss.str();


namespace Phoenix{

	std::string Project::getRelativePath(){
		return Files::getFilePath(path.string());
	}


	void Project::serialize(){
		if(_has_startup_scene){

			if(index()){
				NAML_S serializer{};

				serializer.beginGroup("Project");
					serializer.keyValue("Startup Scene", (std::string)_startup_scene);


					serializer.beginGroup("Assets");
						// scenes
						serializer.beginGroup("Scenes");
							scenes.forEach([&](std::string key, UUID val){
								serializer.keyValue(key, (std::string)val);
							});
						serializer.endGroup();

						// scripts
						serializer.beginGroup("Scripts");
							scripts.forEach([&](std::string key, UUID val){
								serializer.keyValue(key, (std::string)val);
							});
						serializer.endGroup();

						// textures
						serializer.beginGroup("Textures");
							textures.forEach([&](std::string key, UUID val){
								serializer.keyValue(key, (std::string)val);
							});
						serializer.endGroup();

					serializer.endGroup();

				serializer.endGroup();

				Files::writeFile(path.string(), serializer.output());
			#ifdef PH_DEBUG
			}else{
				PH_ERROR("Project Index Failed");
			#endif
			}

		}
	}

	std::string Project::deserialize(std::string filepath, Engine* engine){
		path = filepath;
		return deserialize_functionality(engine);
	}

	std::string Project::deserialize(std::filesystem::path filepath, Engine* engine){
		path = filepath;
		return deserialize_functionality(engine);
	}

	std::string Project::deserialize_functionality(Engine* engine){
		try{
			NAML_DE naml{Files::readFile(path.string())};

			NAML_Node* proj = naml.get()->get("Project");
			
			setStartupScene(proj->get("Startup Scene")->value<UUID>());

			// scenes
			proj->get("Assets")->get("Scenes")->forEach([&](std::string key, NAML_Node* value){
				scenes.insert(key, value->value<UUID>());
			});

			// scripts
			proj->get("Assets")->get("Scripts")->forEach([&](std::string key, NAML_Node* value){
				scripts.insert(key, value->value<UUID>());
			});

			// textures
			proj->get("Assets")->get("Textures")->forEach([&](std::string filepath, NAML_Node* value){
				textures.insert(filepath, value->value<UUID>());
				engine->loadTexture(getRelativePath() + "\\" + filepath, value->value<UUID>());
			});

			return std::string(); 

		}catch(const std::runtime_error& e){
			DESERIALIZE_ERROR("Failed to deserialize (" << path.string() << ")\n\t" << e.what());
			// return false;
		}catch(const std::exception& e){
		    DESERIALIZE_ERROR("Failed to deserialize (" << path.string() << ")\n\t" << e.what());
		    // return false;
		}catch(const std::string& e){
		    DESERIALIZE_ERROR("Failed to deserialize (" << path.string() << ")\n\t" << e);
		    // return false;
		}catch(...){
			DESERIALIZE_ERROR("Failed to deserialize (" << path.string() << ")\n\tUNKNOWN ERROR");
			// return false;
		};
	}

	void Project::setStartupScene(UUID startup_scene){
		_startup_scene = startup_scene;
		_has_startup_scene = true;
		PH_LOG("Set Startup Scene: " << startup_scene);
	}

	UUID Project::getStartupScene() const {
		PH_ASSERT(_has_startup_scene, "No Startup Scene Set\n\tplease create a check before calling Project::getStartupScene()");
		return _startup_scene;
	}


	bool Project::index(){
		

		return true;
	}
	
}


#undef DESERIALIZE_ERROR