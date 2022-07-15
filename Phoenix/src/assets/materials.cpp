#include "ph_pch.h"
#include "materials.h"

namespace Phoenix{

	void BasicMaterial::setTexture(UUID filepath){
		_texture = filepath;
		_using_texture = true;
	}

	void BasicMaterial::useColor(){
		_using_texture = false;
	}

}