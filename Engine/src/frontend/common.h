#pragma once

#include "../../include/Phoenix/defines.h"

#include <stdint.h>

namespace ph{
		

	struct Material3D{ uint32_t id;	};
	struct Material2D{ uint32_t id;	};

	struct Mesh3D{ uint32_t id;	};
	struct Mesh2D{ uint32_t id;	};

	struct TextureID{ uint32_t id; };


	struct StaticCollider{ uint32_t id; };
	struct DynamicCollider{ uint32_t id; };
	struct CharacterController{ uint32_t id; };


};
