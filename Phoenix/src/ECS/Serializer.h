#pragma once



namespace Phoenix{
	class Scene;
	
	class Serializer{
		public:
			Serializer() = default;
			~Serializer() = default;


			void serialize(Scene* scene, const std::string& filepath);
			
			bool deserialize(Scene* scene, const std::string& filepath);
	};

}
