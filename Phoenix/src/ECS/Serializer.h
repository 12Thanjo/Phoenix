#pragma once



namespace Phoenix{
	class Scene;
	class Scripting;
	
	class Serializer{
		public:
			Serializer() = default;
			~Serializer() = default;


			void serialize(Scene* scene, const std::string& filepath);
			
			std::string deserialize(Scene* scene, const std::string& filepath, Scripting& scripting);
	};

}
