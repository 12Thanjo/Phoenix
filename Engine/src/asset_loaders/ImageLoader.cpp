#include "ImageLoader.h"


#if defined(EVO_COMPILER_MSVC)
	#pragma warning(disable : 4244)
#endif

namespace stb{
	#define STB_IMAGE_IMPLEMENTATION
	#include <stb_image.h>
}

#if defined(EVO_COMPILER_MSVC)
	#pragma warning(default : 4244)
#endif



#include "Logging.h"
#include <format>


#include "vulkan/Renderer.h"


namespace ph{
	namespace assets{


		auto ImageLoader::init(vulkan::Renderer* renderer_ptr) noexcept -> bool {
			PH_ASSERT(this->renderer == nullptr, "Image Loader already initialized");


			this->renderer = renderer_ptr;

			stb::stbi_set_flip_vertically_on_load(true);


			PH_INFO("Initialized: Image Loader (stb image)");
			return true;
		};



		auto ImageLoader::shutdown() noexcept -> void {
			PH_ASSERT(this->renderer != nullptr, "Image Loader not initialized");

			this->renderer = nullptr;

			PH_INFO("Shutdown: Image Loader");
		};



		

		auto ImageLoader::load_image(const char* filepath) noexcept -> std::optional<TextureID> {
			PH_ASSERT(this->renderer != nullptr, "Didn't initialize ImageLoader");

			int width;
			int height;
			int channels;
			stb::stbi_uc* pixels = stb::stbi_load(filepath, &width, &height, &channels, 4);
			
			auto texture_result = this->renderer->create_texture(evo::ArrayProxy<byte>{pixels, size_t(width * height * 4)}, width, height, true);

			stb::stbi_image_free(pixels);


			if(texture_result.has_value() == false){
				PH_ERROR(std::format("Failed to load image: '{}'", filepath));
				return std::nullopt;
			}

			return TextureID{ uint32_t(*texture_result) };
		};


	};
};