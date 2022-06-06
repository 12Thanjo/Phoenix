#pragma once



namespace Phoenix{
	class Environment;
	
	class Serializer{
		public:
			Serializer() = default;
			~Serializer() = default;


			void serialize(Environment* environment, const std::string& filepath);
			
			void deserialize(Environment* environment, const std::string& filepath);
	};

}
