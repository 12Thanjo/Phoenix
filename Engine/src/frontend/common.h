#pragma once

#include "../../include/Phoenix/defines.h"

#include <stdint.h>

namespace ph{
	
	using MaterialID = uint32_t;
	using MeshID = uint32_t;
	using TextureID = uint32_t;

	// type aliases to go over the DLL boundary
	namespace alias{
		using Vec4 = void*;
		using Mat4 = void*;


		template<typename T>
		PH_NODISCARD constexpr auto as(void* ptr) noexcept -> T& { return *reinterpret_cast<T*>(ptr); };
	};
};
