#pragma once


#include <libs/Evo/Evo.h>

#include "frontend/common.h"
#include "vulkan/utils.h"


namespace ph{

	namespace assets{

		class MeshLoader{
			public:
				MeshLoader() = default;
				~MeshLoader() = default;


				EVO_NODISCARD auto init() noexcept -> bool;
				auto shutdown() noexcept -> void;


				EVO_NODISCARD auto load_mesh(const char* filepath) noexcept
					-> std::optional< std::pair<std::vector<vulkan::Vertex3D>, std::vector<uint32_t>> >;

		
			private:

		};

	};
};
