#include "FontLoader.h"



#include "Logging.h"
#include <format>


#if defined(EVO_COMPILER_MSVC)
	#pragma warning(disable : 4458)
	#pragma warning(disable : 4505)
#endif

#define MSDF_ATLAS_PUBLIC
// #include <ext/save-png.h>
#include <msdf-atlas-gen.h>
#include <GlyphGeometry.h>
// #include <msdfgen.h>

#if defined(EVO_COMPILER_MSVC)
	#pragma warning(default : 4458)
	#pragma warning(disable : 4505)
#endif


namespace ph{
	namespace assets{

		struct FontLoaderData{
			msdfgen::FreetypeHandle* freetype_handle = nullptr;

			std::vector<msdf_atlas::GlyphGeometry> glyphs{};
			msdf_atlas::FontGeometry font_geometry;
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


			this->data->font_geometry = msdf_atlas::FontGeometry(&this->data->glyphs);

			constexpr double font_scale = 1.0;
			int glyphs_loaded = this->data->font_geometry.loadCharset(font, font_scale, charset);
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
			atlas_packer.setScale(em_size); // em size

			int remaining = atlas_packer.pack(this->data->glyphs.data(), int(this->data->glyphs.size()));
			if(remaining > 0){
				PH_WARNING(std::format("Couldn't pack atlas ({} remaining)", remaining));
			}

			int width, height;
			atlas_packer.getDimensions(width, height);
			em_size = atlas_packer.getScale();


			PH_WARNING(std::format("fix me: {}() [line: {}]", __FUNCTION__, __LINE__));

			{

				#define LCG_MULTIPLIER 6364136223846793005ull
				#define LCG_INCREMENT 1442695040888963407ull
				#define DEFAULT_ANGLE_THRESHOLD 3.0

				constexpr bool expensiveColoring = false;
				constexpr uint64_t coloringSeed = 0;
				const auto edgeColoring = msdfgen::edgeColoringInkTrap;

				if (expensiveColoring) {
				    msdf_atlas::Workload([&](int i, int threadNo) -> bool {
				        unsigned long long glyphSeed = (LCG_MULTIPLIER*(coloringSeed^i)+LCG_INCREMENT)*!!coloringSeed;
				        this->data->glyphs[i].edgeColoring(edgeColoring, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				        return true;
				    }, this->data->glyphs.size()).finish(thread_count);
				} else {
				    unsigned long long glyphSeed = coloringSeed;
				    for (msdf_atlas::GlyphGeometry &glyph : this->data->glyphs) {
				        glyphSeed *= LCG_MULTIPLIER;
				        glyph.edgeColoring(edgeColoring, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				    }
				}

			}







			auto generator_attributes = msdf_atlas::GeneratorAttributes{};
			generator_attributes.config.overlapSupport = true;
			generator_attributes.scanlinePass = true;

			auto generator = msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<evo::byte, 3>>{
				width, height
			};
			generator.setAttributes(generator_attributes);
			generator.setThreadCount(thread_count);
			generator.generate(this->data->glyphs.data(), int(this->data->glyphs.size()));

			const auto bitmap = static_cast<msdfgen::BitmapConstRef<evo::byte, 3>>(generator.atlasStorage());


			auto output = FontData{
				.data 	= {},
				.width  = width,
				.height = height,
			};

			// convert RGB to RGBA
			output.data.reserve(width * height / 3 * 4);

			for(int i = 0; i < width * height * 3; i+=3){
				output.data.push_back(bitmap.pixels[i + 0]);
				output.data.push_back(bitmap.pixels[i + 1]);
				output.data.push_back(bitmap.pixels[i + 2]);
				output.data.push_back(255);
			}



			PH_INFO(std::format("loaded font: '{}'", filepath));

			return output;
		};


	};
};