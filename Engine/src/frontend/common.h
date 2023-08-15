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


	// type aliases to go over the DLL boundary
	namespace alias{
		using Vec3 = void*;
		using Vec4 = void*;
		using Mat4 = void*;


		template<typename T>
		PH_NODISCARD constexpr auto as(void* ptr) noexcept -> T& { return *reinterpret_cast<T*>(ptr); };
	};
};
