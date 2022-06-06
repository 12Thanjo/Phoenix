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


				serializer.endGroup();
			});


		// serializer.beginGroup("TEST");
			// serializer.keyValue("foo", "bar");
			serialize_vec3(serializer, "foo", glm::vec3(1.0f, 1.0f, 2.0f));
		// serializer.endGroup();		


		serializer.endGroup();


		std::ofstream fout(filepath);
		fout << serializer.output().c_str();



	}



	void Serializer::deserialize(Environment* environment, const std::string& filepath){
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


		PH_WARNING(
			test.get()->get("Entities")->get("foo")->value()
		);

	}

}
