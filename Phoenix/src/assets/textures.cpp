#include "ph_pch.h"
#include "textures.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


namespace Phoenix{
	
	Texture::Texture(std::string filepath)
		: _filepath(filepath){

		stbi_set_flip_vertically_on_load(1);

		int width, height, nrChanels;
		unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChanels, 4);


		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		if(data){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}else{
			PH_WARNING("Failed to load texture: " << _filepath);
		};
		stbi_image_free(data);

		PH_LOG("Loaded Texture: " << _filepath << " (glID: " << _id << ")");
	}


	Texture::~Texture(){
		glDeleteTextures(1, &_id);
	}



	void Texture::bind(unsigned int slot){
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	void Texture::unbind(){
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
