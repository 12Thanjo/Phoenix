#include "Project.h"

#include "ImGui helpers.h"


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

		}else{
			imgui_start_alert("No startup scene is set\nThis can be set in the Scene tab under \"General\"");
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
		std::string relative_path = Files::getFilePath(path.string());

		//////////////////////////////////////////////////////////////////////
		// make sure all necesary folders are there

		bool has_scenes = false;
		bool has_scripts = false;
		bool has_textures = false;
		Files::directoryIterator(relative_path, [&](const std::string& path, bool is_dir){
			if(is_dir){
				std::string folder_name = Files::getFileName(path);

					 if(folder_name == "scenes")   { has_scenes   = true; }
				else if(folder_name == "scripts")  { has_scripts  = true; }
				else if(folder_name == "textures") { has_textures = true; }

			}
		});

		#define PH_PROJECT_INDEX_CHECK_EXISTS(folder) if(!has_##folder){ \
				imgui_start_alert("Project Directory is missing the "#folder" folder.\nPlease add one and try to save again."); \
				return false; \
			}

			PH_PROJECT_INDEX_CHECK_EXISTS(scenes);
			PH_PROJECT_INDEX_CHECK_EXISTS(scripts);
			PH_PROJECT_INDEX_CHECK_EXISTS(textures);

		#undef PH_PROJECT_INDEX_CHECK_EXISTS

		//////////////////////////////////////////////////////////////////////
		// check if all scenes are indexed

		// scenes
		Files::directoryIterator(relative_path + "\\scenes", [&](const std::string& path, bool is_dir){
			std::string relative_filepath = Files::relative(path, relative_path);

			if(Files::getFileExtention(relative_filepath) == "phoenix_scene" && !scenes.hasLeft(relative_filepath)){
				scenes.insert(
					relative_filepath,
					{ std::stoull(
						Files::readFile(path).substr(8, 28)
					) }
				);
			}
		});


		// scripts
		Files::directoryIterator(relative_path + "\\scripts", [&](const std::string& path, bool is_dir){
			std::string relative_filepath = Files::relative(path, relative_path);

			if(Files::getFileExtention(relative_filepath) == "js" && !scripts.hasLeft(relative_filepath)){
				scripts.insert(
					relative_filepath,
					UUID() // if new script found, generate new UUID
				);
			}
		});


		// textures
		Files::directoryIterator(relative_path + "\\textures", [&](const std::string& path, bool is_dir){
			std::string relative_filepath = Files::relative(path, relative_path);

			if(Files::getFileExtention(relative_filepath) == "png" && !textures.hasLeft(relative_filepath)){
				textures.insert(
					relative_filepath,
					UUID() // if new texture found, generate new UUID
				);
			}
		});


		PH_WARNING("Unimplemented: removing deleted files from index");
		PH_TRACE();


		//////////////////////////////////////////////////////////////////////
		// all passed

		return true;
	}
	
}


#undef DESERIALIZE_ERROR