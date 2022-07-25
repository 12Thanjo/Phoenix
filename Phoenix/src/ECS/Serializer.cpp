#include "ph_pch.h"
#include "Serializer.h"

#include "Scene.h"
#include "components.h"
#include "Entity.h"

#include "src/naml/Serialize.h"
#include "src/naml/Deserialize.h"

#include "src/scripting/scripting.h"


#define DESERIALIZE_ERROR(msg) PH_WARNING(msg); \
	std::stringstream ss; \
	ss << msg; \
	return ss.str();


namespace Phoenix{

	static void serialize_basic_material(NAML_S& serializer, BasicMaterial& material){
		serializer.beginGroup("Basic Material");
			if(material.usingTexture()){
				serializer.keyValue("texture", material.getTexture());
			}
			serializer.keyValue("color", material.color);
			serializer.keyValue("shininess", material.shininess);
		serializer.endGroup();
	}



	void Serializer::serialize(Scene* scene, const std::string& filepath){
		NAML_S serializer{};

		serializer.keyValue("Scene", scene->uuid);
		serializer.keyValue("Name", scene->name);

		if(scene->hasStartupCamera()){
			serializer.keyValue("Startup Camera", scene->getStartupCamera());
		}


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
			serializer.keyValue("focalPoint", editor_camera.getFocalPoint());
		serializer.endGroup();


		//////////////////////////////////////////////////////////////////////
		// Sunlight

		serializer.beginGroup("Sunlight");
			Lights::Directional& sunlight = scene->sunlight;
			serializer.keyValue("direction", sunlight.direction);
			serializer.keyValue("color", sunlight.color);
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
						serializer.keyValue("position", component.position);
						serializer.keyValue("rotation", component.rotation);
						serializer.keyValue("scale", component.scale);
					serializer.endGroup();
				}

				if(entity.hasComponent<Component::Script>()){
					serializer.keyValue("Script", entity.getComponent<Component::Script>().path);
				}

				if(entity.hasComponent<Component::PointLight>()){
					Lights::Point& light = entity.getComponent<Component::PointLight>().light;

					serializer.beginGroup("PointLight");
						serializer.keyValue("ambient", light.ambient);
						serializer.keyValue("diffuse", light.diffuse);
						serializer.keyValue("specular", light.specular);
						serializer.keyValue("strength", light.strength);
					serializer.endGroup();
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
						serializer.keyValue("focalPoint", component.camera.getFocalPoint());
					serializer.endGroup();
				}


				//////////////////////////////////////////////////////////////////////
				// Rendering

				if(entity.hasComponent<Component::Sprite>()){
					serializer.beginGroup("Sprite");
						Component::Sprite& sprite = entity.getComponent<Component::Sprite>();
						serializer.keyValue("color", sprite.color);
						if(sprite.using_texture && sprite.has_texture){
							serializer.keyValue("texture", sprite.texture);
						}
					serializer.endGroup();
				}
				
				if(entity.hasComponent<Component::Cube>()){
					serializer.beginGroup("Cube");
						serialize_basic_material(serializer, entity.getComponent<Component::Cube>().material);
					serializer.endGroup();
				}

				if(entity.hasComponent<Component::Plane>()){
					serializer.beginGroup("Plane");
						serialize_basic_material(serializer, entity.getComponent<Component::Plane>().material);
					serializer.endGroup();
				}




				serializer.endGroup();
			});


		serializer.endGroup();

		// std::ofstream fout(filepath);
		// fout << serializer.output().c_str();

		Files::writeFile(filepath, serializer.output().c_str());
	}


	//////////////////////////////////////////////////////////////////////
	// deserializing

	template<typename T>
	static void deserialize_basic_material(NAML_Node* material_node, Entity& entity){

		T& component = entity.addComponent<T>(
			BasicMaterial(
				material_node->get("color")->value<glm::vec4>(),
				material_node->get("shininess")->value<float>()
			)
		);

		if(material_node->has("texture")){
			component.material.setTexture(material_node->get("texture")->value<UUID>());
			component.using_texture = true;
		}
	}



	std::string Serializer::deserialize(Scene* scene, const std::string& filepath, Scripting& scripting){
		try{
			// check file exists
			if(!Files::fileExists(filepath, [&](std::string error){
				throw error;
			})){
				throw std::string("Filepath does not exists");
			}

			//////////////////////////////////////////////////////////
			// deserialize
			NAML_DE naml{Files::readFile(filepath)};

			scene->name = naml.get()->get("Name")->value<std::string>();
			scene->uuid = { naml.get()->get("Scene")->value<UUID>()};

			if(naml.get()->has("Startup Camera")){
				scene->setStartupCamera( naml.get()->get("Startup Camera")->value<UUID>() );
			}

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
						node->get("Script")->value<std::string>(),
						Files::getFilePathUpDirectory(Files::getFilePath(filepath)) + "\\" + node->get("Script")->value<std::string>(),
						scripting,
						entity.getComponent<Component::UUID>().id
					);
				}

				if(node->has("PointLight")){
					Lights::Point& light = entity.addComponent<Component::PointLight>().light;

					NAML_Node* light_node = node->get("PointLight");

					light.ambient = light_node->get("ambient")->value<glm::vec3>();
					light.diffuse = light_node->get("diffuse")->value<glm::vec3>();
					light.specular = light_node->get("specular")->value<glm::vec3>();
					light.strength = light_node->get("strength")->value<float>();
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

				if(node->has("Sprite")){
					NAML_Node* sprite_node = node->get("Sprite");
					Component::Sprite& sprite = entity.addComponent<Component::Sprite>(
						sprite_node->get("color")->value<glm::vec4>()
					);

					if(sprite_node->has("texture")){
						sprite.texture = sprite_node->get("texture")->value<UUID>();
						sprite.using_texture = true;
						sprite.has_texture = true;
					}
				}

				if(node->has("Cube")){
					deserialize_basic_material<Component::Cube>(node->get("Cube")->get("Basic Material"), entity);
				}

				if(node->has("Plane")){
					deserialize_basic_material<Component::Plane>(node->get("Plane")->get("Basic Material"), entity);
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
