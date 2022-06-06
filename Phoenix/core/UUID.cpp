#include "ph_pch.h"
#include "UUID.h"

#include <random>

namespace Phoenix{

	static std::random_device random_device;
	static std::mt19937_64 engine(random_device());
	static std::uniform_int_distribution<uint64_t> uniform_distribution;


	
	UUID::UUID()
		: _id(uniform_distribution(engine)) {

	}


	UUID::UUID(uint64_t id)
		: _id(id) {

	}

	// UUID generateUUID(){
	// 	return uniform_distribution(engine);
	// }

}
