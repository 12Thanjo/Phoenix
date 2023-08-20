#pragma once


#include <libs/Evo/Evo.h>

#include "frontend/common.h"


namespace ph{
	// forward declaration
	namespace vulkan{ class Renderer; };


	namespace assets{

		class ImageLoader{
			public:
				ImageLoader() = default;
				~ImageLoader() = default;


				EVO_NODISCARD auto init(vulkan::Renderer* renderer_ptr) noexcept -> bool;
				auto shutdown() noexcept -> void;


				EVO_NODISCARD auto load_image(const char* filepath) noexcept -> std::optional<TextureID>;

		
			private:
				vulkan::Renderer* renderer = nullptr;
		};

	};
};