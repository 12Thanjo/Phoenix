#include "FontLoader.h"



#include "Logging.h"
#include <format>


#if defined(EVO_COMPILER_MSVC)
	#pragma warning(push, 0)
#endif

#define MSDF_ATLAS_PUBLIC
// #include <ext/save-png.h>
#include <msdf-atlas-gen.h>
#include <GlyphGeometry.h>
// #include <msdfgen.h>

#if defined(EVO_COMPILER_MSVC)
	#pragma warning(pop)
#endif


namespace ph{
	namespace assets{

		struct FontData{
			std::vector<msdf_atlas::GlyphGeometry> glyphs{};
			msdf_atlas::FontGeometry geometry;
			int width;
			int height;
		};

		struct FontLoaderData{
			msdfgen::FreetypeHandle* freetype_handle = nullptr;

			std::vector<FontData> font_data{ FontData{} };
		};


		auto FontLoader::init() noexcept -> bool {
			PH_ASSERT(this->data == nullptr, "Font Loader was already initialized");

			this->data = new FontLoaderData{};

			this->data->freetype_handle = msdfgen::initializeFreetype();
			if(this->data->freetype_handle == nullptr){
				PH_ERROR("Failed to initialze Freetype");
				return false;
			}


			PH_INFO("Initialized: Text Loader (msdfgen)");
			return true;
		};



		auto FontLoader::shutdown() noexcept -> void {
			PH_ASSERT(this->data != nullptr, "Font Loader is not initialized");

			if(this->data->freetype_handle != nullptr){ msdfgen::deinitializeFreetype(this->data->freetype_handle); }

			delete this->data;
			this->data = nullptr;

			PH_INFO("Shutdown: Text Loader");
		};



		
		auto FontLoader::load_text(const char* filepath) noexcept -> std::optional<FontData> {
			PH_ASSERT(this->data != nullptr, "Font Loader is not initialized");

			constexpr int thread_count = 2;
			
			msdfgen::FontHandle* font = msdfgen::loadFont(this->data->freetype_handle, filepath);
			if(font == nullptr){
				PH_ERROR(std::format("Failed to load font: '{}'", filepath));
				return std::nullopt;
			}


			auto charset = msdf_atlas::Charset{};
			for(uint32_t i = 0x0020; i < 0x00FF; i+=1){
				charset.add(i);
			}

			auto& font_data = this->data->font_data[0];

			font_data.geometry = msdf_atlas::FontGeometry(&font_data.glyphs);

			constexpr double font_scale = 1.0;
			int glyphs_loaded = font_data.geometry.loadCharset(font, font_scale, charset);
			PH_TRACE(std::format("Loaded {}/{} glyphs from font '{}'", glyphs_loaded, charset.size(), filepath));

			if(glyphs_loaded == 0){
				PH_ERROR("Loaded 0 glyphs from font");
				return std::nullopt;
			}

			msdfgen::destroyFont(font);


			double em_size = 40.0;

			auto atlas_packer = msdf_atlas::TightAtlasPacker{};
			// atlas_packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::POWER_OF_TWO_RECTANGLE);
			atlas_packer.setPixelRange(2.0);
			atlas_packer.setMiterLimit(1.0);
			atlas_packer.setPadding(0);
			atlas_packer.setScale(em_size);

			const int chars_remaining = atlas_packer.pack(font_data.glyphs.data(), int(font_data.glyphs.size()));
			if(chars_remaining > 0){
				PH_WARNING(std::format("Couldn't pack atlas ({} remaining)", chars_remaining));
			}

			atlas_packer.getDimensions(font_data.width, font_data.height);
			em_size = atlas_packer.getScale();


		    for(msdf_atlas::GlyphGeometry& glyph : font_data.glyphs){
		    	// constexpr auto func = msdfgen::edgeColoringInkTrap;
		    	constexpr auto func = msdfgen::edgeColoringByDistance;
				constexpr double default_angle_threshold = 3.0;
				constexpr uint64_t glyph_seed = 0;	

		        glyph.edgeColoring(func, default_angle_threshold, glyph_seed);
		    }



			auto generator_attributes = msdf_atlas::GeneratorAttributes{};
			generator_attributes.config.overlapSupport = true;
			generator_attributes.scanlinePass = true;

			auto generator = msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<evo::byte, 3>>{
				font_data.width, font_data.height
			};
			generator.setAttributes(generator_attributes);
			generator.setThreadCount(thread_count);
			generator.generate(font_data.glyphs.data(), int(font_data.glyphs.size()));

			const auto bitmap = static_cast<msdfgen::BitmapConstRef<evo::byte, 3>>(generator.atlasStorage());


			auto output = FontData{
				.data 	= std::vector<evo::byte>{},
				.width  = font_data.width,
				.height = font_data.height,
			};


			// convert RGB to RGBA
			output.data.reserve(font_data.width * font_data.height / 3 * 4);

			for(int i = 0; i < font_data.width * font_data.height * 3; i+=3){
				output.data.push_back(bitmap.pixels[i + 0]);
				output.data.push_back(bitmap.pixels[i + 1]);
				output.data.push_back(bitmap.pixels[i + 2]);
				output.data.push_back(255);
			}



			PH_INFO(std::format("loaded font: '{}'", filepath));

			return output;
		};



		auto FontLoader::get_char_bounds(const std::string_view string) noexcept -> std::optional<std::vector<CharBounds>> {
			constexpr float line_height_offset = 0.0f;
			constexpr float kerning_offset = 0.0f;

			auto output = std::vector<CharBounds>{};
			output.reserve(string.size());


			auto& font_data = this->data->font_data[0];

			const auto& metrics = font_data.geometry.getMetrics();

			double x = 0.0;
			double y = 0.0;

			const double font_scale = 1.0 / (metrics.ascenderY - metrics.descenderY);

			for(int i = 0; i < string.size(); i+=1){
				char character = string[i];



				switch(character){
					case '\n': {
						x = 0;
						y -= font_scale * metrics.lineHeight + line_height_offset;
						continue;
					} break;

					case '\r': {
						continue;
					} break;

					case ' ': {
						const auto* space_glyph = font_data.geometry.getGlyph(character);
						double advance = space_glyph->getAdvance();
						const char next_character = string[i+1];
						font_data.geometry.getAdvance(advance, character, next_character);

						x += font_scale * advance + kerning_offset;
						continue;
					} break;

					case '\t': {
						// find the spacing for the first 3 spaces of the tab
						const auto* space_glyph = font_data.geometry.getGlyph(' ');
						double advance = space_glyph->getAdvance();
						font_data.geometry.getAdvance(advance, ' ', ' ');

						x += (font_scale * advance + kerning_offset) * 3;


						// find the spacing for the last space of the tab and the next character						
						const char next_character = string[i+1];
						font_data.geometry.getAdvance(advance, ' ', next_character);

						x += font_scale * advance + kerning_offset;

						continue;
					} break;
				};


				auto* glyph = font_data.geometry.getGlyph(character);
				if(!glyph){ glyph = font_data.geometry.getGlyph('?'); }
				if(!glyph){
					// TODO: better error message
					PH_ERROR(std::format("Cannot find glyph '{}' or replacement ('?')", character));
					return std::nullopt;
				}



				struct PlaneBounds {
					double left;
					double right;
					double top;
					double bottom;
				} plane;

				glyph->getQuadPlaneBounds(plane.left, plane.bottom, plane.right, plane.top);

				const auto quad_min = glm::vec2{float(plane.left), float(plane.bottom)} * float(font_scale) + glm::vec2(x, y);
				const auto quad_max = glm::vec2{float(plane.right), float(plane.top)} * float(font_scale) + glm::vec2(x, y);



				struct AtlasBounds {
					double left;
					double right;
					double top;
					double bottom;
				} atlas;

				glyph->getQuadAtlasBounds(atlas.left, atlas.bottom, atlas.right, atlas.top);

				const float texel_width = 1.0f / font_data.width;
				const float texel_height = 1.0f / font_data.height;

				const auto tex_coord_min = glm::vec2{float(atlas.left), float(atlas.bottom)} * glm::vec2{texel_width, texel_height};
				const auto tex_coord_max = glm::vec2{float(atlas.right), float(atlas.top)} * glm::vec2{texel_width, texel_height};


				output.emplace_back() = {
					.x = quad_min.x,
					.y = quad_min.y,

					.width = quad_max.x - quad_min.x,
					.height = quad_max.y - quad_min.y,

					.min = tex_coord_min,
					.max = tex_coord_max,
				};


				if(i + 1 < string.size()){
					double advance = glyph->getAdvance();
					char next_character = string[i+1];
					font_data.geometry.getAdvance(advance, character, next_character);

					x += font_scale * advance + kerning_offset;
				}
			}


			return output;
		};


	};
};