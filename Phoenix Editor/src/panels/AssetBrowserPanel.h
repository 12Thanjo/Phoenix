#pragma once

#include <filesystem>

#include "Panel.h"


namespace Phoenix{
	
	class AssetBrowserPanel : public Panel{
		public:
			AssetBrowserPanel();
			~AssetBrowserPanel();
	
			void render(Engine* editor) override;

		public:
			bool visible = true;
		
		private:
			std::filesystem::path _current_dir;

			Texture* _folder_icon;
			Texture* _gray_file_icon;
	};

}
