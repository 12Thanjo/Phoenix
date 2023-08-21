#pragma once


#include <libs/Evo/Evo.h>



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

		
			private:
				struct FontLoaderData* data = nullptr;
		};

	};
};
