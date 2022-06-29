#include "Project.h"

#include "ImGui helpers.h"


#define DESERIALIZE_ERROR(msg) PH_WARNING(msg); \
	std::stringstream ss; \
	ss << msg; \
	return ss.str();


namespace Phoenix{

	void Project::serialize(){
		if(_has_startup_scene){
			NAML_S serializer{};

			serializer.beginGroup("Project");
				serializer.keyValue("Startup Scene", (std::string)_startup_scene);

			serializer.endGroup();


			Files::writeFile(path.string(), serializer.output());
		}else{
			imgui_start_alert("No startup scene is set");
		}
	}

	std::string Project::deserialize(std::string filepath){
		path = filepath;
		return deserialize_functionality();
	}

	std::string Project::deserialize(std::filesystem::path filepath){
		path = filepath;
		return deserialize_functionality();
	}

	std::string Project::deserialize_functionality(){
		try{
			NAML_DE naml{Files::readFile(path.string())};
			
			setStartupScene(naml.get()->get("Project")->get("Startup Scene")->value<UUID>());

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
	
}


#undef DESERIALIZE_ERROR