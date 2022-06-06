#pragma once


namespace Phoenix{
	
	class Texture{
		private:
			std::string _filepath;
			glID _id;
	
		public:
			Texture(std::string filepath);
			~Texture();


			void bind(unsigned int slot = 0);
			void unbind();
			
	};

}
