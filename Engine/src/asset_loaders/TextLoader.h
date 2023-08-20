#pragma once


#include <libs/Evo/Evo.h>



namespace ph{

	namespace assets{

		class TextLoader{
			public:
				TextLoader() = default;
				~TextLoader() = default;


				EVO_NODISCARD auto init() noexcept -> bool;
				auto shutdown() noexcept -> void;


				EVO_NODISCARD auto load_text(const char* filepath) noexcept -> void;

		
			private:

		};

	};
};
