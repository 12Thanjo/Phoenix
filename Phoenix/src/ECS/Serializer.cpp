#include "ph_pch.h"
#include "Serializer.h"

#include "Scene.h"
#include "components.h"
#include "Entity.h"
#include "src/naml/Serialize.h"
#include "src/naml/Deserialize.h"


#define DESERIALIZE_ERROR(msg) PH_WARNING(msg); \
	std::stringstream ss; \
	ss << msg; \
	return ss.str();


namespace Phoenix{


	static void serialize_vec3(NAML_S& serializer, const std::string& key, const glm::vec3& vec3){
		serializer.beginList(key);
			serializer.addToList(vec3.x);
			serializer.addToList(vec3.y);
			serializer.addToList(vec3.z);
		serializer.endList();
	}

	static void serialize_vec4(NAML_S& serializer, const std::string& key, const glm::vec4& vec4){
		serializer.beginList(key);
			serializer.addToList(vec4.x);
			serializer.addToList(vec4.y);
			serializer.addToList(vec4.z);
			serializer.addToList(vec4.w);
		serializer.endList();
	}



	void Serializer::serialize(Scene* scene, const std::string& filepath){
		NAML_S serializer{};

		serializer.keyValue("Scene", std::to_string(scene->uuid));
		serializer.keyValue("Name", scene->name);


		//////////////////////////////////////////////////////////////////////
		// Serialize Editor Camera

		serializer.beginGroup("Editor Camera");
			OrbitalCamera& editor_camera = scene->camera;
			serializer.keyValue("fov", editor_camera.getFOV());
			serializer.keyValue("near", editor_camera.getNear());
			serializer.keyValue("far", editor_camera.getFar());
			serializer.beginList("coordinates");
				serializer.addToList(editor_camera.getRho());
				serializer.addToList(editor_camera.getTheta());
				serializer.addToList(editor_camera.getPhi());
			serializer.endList();
			serialize_vec3(serializer, "focalPoint", editor_camera.getFocalPoint());
		serializer.endGroup();


		//////////////////////////////////////////////////////////////////////
		// Sunlight

		serializer.beginGroup("Sunlight");
			Lights::Directional& sunlight = scene->sunlight;
			serializer.beginList("direction");
				serializer.addToList(sunlight.direction.x);
				serializer.addToList(sunlight.direction.y);
				serializer.addToList(sunlight.direction.z);
			serializer.endList();
			serializer.beginList("color");
				serializer.addToList(sunlight.color.x);
				serializer.addToList(sunlight.color.y);
				serializer.addToList(sunlight.color.z);
			serializer.endList();
			serializer.keyValue("strength", sunlight.strength);
		serializer.endGroup();


		//////////////////////////////////////////////////////////////////////
		// Serialize Entities

		serializer.beginGroup("Entities");
			scene->forEach([&](Entity entity){
				serializer.beginGroup(entity.getComponent<Component::UUID>().id);

				serializer.keyValue("name", entity.getComponent<Component::Name>().name);

				if(entity.hasComponent<Component::Transform>()){
					auto& component = entity.getComponent<Component::Transform>();

					serializer.beginGroup("Transform");
						serialize_vec3(serializer, "position", component.position);
						serialize_vec3(serializer, "rotation", component.rotation);
						serialize_vec3(serializer, "scale", component.scale);
					serializer.endGroup();
				}

				if(entity.hasComponent<Component::Script>()){
					serializer.keyValue("Script", entity.getComponent<Component::Script>().path);
				}

				//////////////////////////////////////////////////////////////////////
				// Cameras

				if(entity.hasComponent<Component::PerspectiveCamera>()){
					auto& component = entity.getComponent<Component::PerspectiveCamera>();

					serializer.beginGroup("PerspectiveCamera");
						serializer.keyValue("fov", component.camera.getFOV());
						serializer.keyValue("near", component.camera.getNear());
						serializer.keyValue("far", component.camera.getFar());
					serializer.endGroup();
				}


				if(entity.hasComponent<Component::OrbitalCamera>()){
					auto& component = entity.getComponent<Component::OrbitalCamera>();

					serializer.beginGroup("OrbitalCamera");
						serializer.keyValue("fov", component.camera.getFOV());
						serializer.keyValue("near", component.camera.getNear());
						serializer.keyValue("far", component.camera.getFar());
						serializer.beginList("coordinates");
							serializer.addToList(component.camera.getRho());
							serializer.addToList(component.camera.getTheta());
							serializer.addToList(component.camera.getPhi());
						serializer.endList();
						serialize_vec3(serializer, "focalPoint", component.camera.getFocalPoint());
					serializer.endGroup();
				}


				//////////////////////////////////////////////////////////////////////
				// Rendering

				if(entity.hasComponent<Component::SpriteRenderer>()){
					serialize_vec4(serializer, "SpriteRenderer", entity.getComponent<Component::SpriteRenderer>().color);
				}
				
				if(entity.hasComponent<Component::Cube>()){
					serialize_vec4(serializer, "Cube", entity.getComponent<Component::Cube>().color);
				}




				serializer.endGroup();
			});


		serializer.endGroup();

		// std::ofstream fout(filepath);
		// fout << serializer.output().c_str();

		Files::writeFile(filepath, serializer.output().c_str());
	}



	std::string Serializer::deserialize(Scene* scene, const std::string& filepath){
		try{

			//////////////////////////////////////////////////////////
			// deserialize
			NAML_DE naml{Files::readFile(filepath)};

			scene->name = naml.get()->get("Name")->value<std::string>();
			scene->uuid = { naml.get()->get("Scene")->value<UUID>()};

			//////////////////////////////////////////////////////////////////////
			// Deserialize Editor Camera

			OrbitalCamera& editor_camera = scene->camera;
			NAML_Node* camera_node = naml.get()->get("Editor Camera");
			editor_camera.setProjection(
				camera_node->get("fov")->value<float>(),
				1.0f, //aspect ratio
				camera_node->get("near")->value<float>(),
				camera_node->get("far")->value<float>()
			);

			glm::vec3 coordinates = camera_node->get("coordinates")->value<glm::vec3>();
			editor_camera.setCoordinates(coordinates.x, coordinates.y, coordinates.z);


			//////////////////////////////////////////////////////////////////////
			// Sunlight

			Lights::Directional& sunlight = scene->sunlight;
			NAML_Node* sunlight_node = naml.get()->get("Sunlight");
			sunlight.color = sunlight_node->get("color")->value<glm::vec3>();
			sunlight.direction = sunlight_node->get("direction")->value<glm::vec3>();
			sunlight.strength = sunlight_node->get("strength")->value<float>();


			//////////////////////////////////////////////////////////////////////
			// Deserialize Entities

			naml.get()->get("Entities")->forEach([&](std::string id, NAML_Node* node){
				Entity entity = scene->createEntity( node->get("name")->value<std::string>(), {std::stoull(id)} );


				if(node->has("Transform")){
					NAML_Node* component = node->get("Transform");
					entity.addComponent<Component::Transform>(
						component->get("position")->value<glm::vec3>(),
						component->get("rotation")->value<glm::vec3>(),
						component->get("scale")->value<glm::vec3>()
					);
				}

				if(node->has("Script")){
					entity.addComponent<Component::Script>(
						node->get("Script")->value<std::string>()
					);
				}

				//////////////////////////////////////////////////////////////////////
				// Cameras

				if(node->has("PerspectiveCamera")){
					NAML_Node* component = node->get("PerspectiveCamera");
					entity.addComponent<Component::PerspectiveCamera>(
						component->get("fov")->value<float>(),
						1.0f, //aspect ratio
						component->get("near")->value<float>(),
						component->get("far")->value<float>()
					);

				}

				if(node->has("OrbitalCamera")){
					NAML_Node* component = node->get("OrbitalCamera");
					entity.addComponent<Component::OrbitalCamera>(
						component->get("fov")->value<float>(),
						1.0f, //aspect ratio
						component->get("near")->value<float>(),
						component->get("far")->value<float>()
					);

					auto& camera_component = entity.getComponent<Component::OrbitalCamera>();

					
					glm::vec3 coordinates = component->get("coordinates")->value<glm::vec3>();
					camera_component.camera.setCoordinates(coordinates.x, coordinates.y, coordinates.z);

					camera_component.camera.setFocalPoint(component->get("focalPoint")->value<glm::vec3>());

				}


				//////////////////////////////////////////////////////////////////////
				// rendering

				if(node->has("SpriteRenderer")){
					entity.addComponent<Component::SpriteRenderer>(
						node->get("SpriteRenderer")->value<glm::vec4>()
					);
				}

				if(node->has("Cube")){
					entity.addComponent<Component::Cube>(
						node->get("Cube")->value<glm::vec4>()
					);
				}	

				// glm::vec3 position = node->get("Transform")->get("scale")->value<glm::vec3>();
				// PH_LOG(position.x << ", " << position.y << ", " << position.z);
			});
		}catch(const std::runtime_error& e){
			DESERIALIZE_ERROR("Failed to deserialize (" << filepath << ")\n\t" << e.what());
			// return false;
		}catch(const std::exception& e){
		    DESERIALIZE_ERROR("Failed to deserialize (" << filepath << ")\n\t" << e.what());
		    // return false;
		}catch(const std::string& e){
		    DESERIALIZE_ERROR("Failed to deserialize (" << filepath << ")\n\t" << e);
		    // return false;
		}catch(...){
			DESERIALIZE_ERROR("Failed to deserialize (" << filepath << ")\n\tUNKNOWN ERROR");
			// return false;
		};

		return std::string();
	}

}
