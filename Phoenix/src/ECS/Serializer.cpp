#include "ph_pch.h"
#include "Serializer.h"

#include "Environment.h"
#include "components.h"
#include "Entity.h"
#include "src/naml/Serialize.h"
#include "src/naml/Deserialize.h"


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



	void Serializer::serialize(Environment* environment, const std::string& filepath){
		NAML_S serializer{};

		serializer.keyValue("Environment", "!Untitled!");
		serializer.beginGroup("Entities");

			environment->forEach([&](Entity entity){
				serializer.beginGroup( std::to_string(entity.getComponent<Component::UUID>().id) );

				serializer.keyValue("name", entity.getComponent<Component::Name>().name);

				if(entity.hasComponent<Component::Transform>()){
					auto component = entity.getComponent<Component::Transform>();

					serializer.beginGroup("Transform");
						serialize_vec3(serializer, "translation", component.translation);
						serialize_vec3(serializer, "rotation", component.rotation);
						serialize_vec3(serializer, "scale", component.scale);
					serializer.endGroup();
				}



				if(entity.hasComponent<Component::SpriteRenderer>()){
					serialize_vec4(serializer, "SpriteRenderer", entity.getComponent<Component::SpriteRenderer>().color);
				}

				if(entity.hasComponent<Component::Camera>()){
					auto component = entity.getComponent<Component::Camera>();

					serializer.beginGroup("Camera");
						serializer.keyValue("fov", component.camera.getFOV());
						serializer.keyValue("near", component.camera.getNear());
						serializer.keyValue("far", component.camera.getFar());
						serializer.keyValue("primary", component.primary);
					serializer.endGroup();
				}

				
				if(entity.hasComponent<Component::Cube>()){
					serialize_vec4(serializer, "Cube", entity.getComponent<Component::Cube>().color);
				}


				serializer.endGroup();
			});


		// serializer.beginGroup("TEST");
			// serializer.keyValue("foo", "bar");
			// serialize_vec3(serializer, "foo", glm::vec3(1.0f, 1.0f, 2.0f));
		// serializer.endGroup();		


		serializer.endGroup();


		std::ofstream fout(filepath);
		fout << serializer.output().c_str();



	}



	void Serializer::deserialize(Environment* environment, const std::string& filepath){
		try{
			//////////////////////////////////////////////////////////
			// load file
			std::ifstream file(filepath);
			std::stringstream stream;

			std::string line;
			while(getline(file, line)){
				stream << line << "\n";
			};


			//////////////////////////////////////////////////////////
			// deserialize
			NAML_DE test{stream.str()};


			test.get()->get("Entities")->forEach([&](std::string id, NAML_Node* node){
				Entity entity = environment->createEntity( node->get("name")->value<std::string>(), {std::stoull(id)} );


				if(node->has("Transform")){
					NAML_Node* component = node->get("Transform");
					entity.addComponent<Component::Transform>(
						component->get("translation")->value<glm::vec3>(),
						component->get("rotation")->value<glm::vec3>(),
						component->get("scale")->value<glm::vec3>()
					);
				}

				if(node->has("Camera")){
					NAML_Node* component = node->get("Camera");
					entity.addComponent<Component::Camera>(
						component->get("fov")->value<float>(),
						1.0f, //aspect ratio
						component->get("near")->value<float>(),
						component->get("far")->value<float>()
					);

					entity.getComponent<Component::Camera>().primary = component->get("primary")->value<bool>();
				}

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
		}catch(...){
			PH_WARNING("Failed to deserialize (" << filepath << ")");
		};

	}

}
