#include "TextLoader.h"



#include "Logging.h"
#include <format>

#define MSDF_ATLAS_PUBLIC
#include <ext/save-png.h>
#include <msdf-atlas-gen.h>
#include <msdfgen.h>


namespace ph{
	namespace assets{


		auto TextLoader::init() noexcept -> bool {

			this->load_text("C:\\Windows\\Fonts\\arialbd.ttf");

			PH_INFO("Initialized: Text Loader (msdfgen)");
			return true;
		};



		auto TextLoader::shutdown() noexcept -> void {

			PH_INFO("Shutdown: Text Loader");
		};



		
		auto TextLoader::load_text(const char* filepath) noexcept -> void {

			msdfgen::FreetypeHandle *ft = msdfgen::initializeFreetype();
		    if (ft) {
		        msdfgen::FontHandle *font = msdfgen::loadFont(ft, filepath);
		        if (font) {
		            msdfgen::Shape shape;
		            if (msdfgen::loadGlyph(shape, font, 'B')) {
		                shape.normalize();
		                //                               max. angle
		                msdfgen::edgeColoringSimple(shape, 3.0);
		                //                    image width, height
		                msdfgen::Bitmap<float, 3> msdf(32, 32);
		                //                               range, scale, translation
		                msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
		                msdfgen::savePng(msdf, "output.png");
		            }
		            msdfgen::destroyFont(font);
		        }
		        msdfgen::deinitializeFreetype(ft);
		    }


		    PH_INFO(std::format("loaded font: '{}'", filepath));
		};


	};
};