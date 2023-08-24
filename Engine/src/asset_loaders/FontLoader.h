#pragma once


#include <libs/Evo/Evo.h>

#include <libs/glm/glm.h>

namespace ph{
	namespace assets{

		class FontLoader{
			public:
				FontLoader() = default;
				~FontLoader() = default;


				EVO_NODISCARD auto init() noexcept -> bool;
				auto shutdown() noexcept -> void;


				struct FontData{
					std::vector<evo::byte> data;
					int width;
					int height;
				};
				EVO_NODISCARD auto load_text(const char* filepath) noexcept -> std::optional<FontData>;



				struct CharBounds{
					float x;
					float y;

					float width;
					float height;

					glm::vec2 min;
					glm::vec2 max;
				};

				auto get_char_bounds(const std::string_view string) noexcept -> std::optional<std::vector<CharBounds>>;

		
			private:
				struct FontLoaderData* data = nullptr;
		};

	};
};
