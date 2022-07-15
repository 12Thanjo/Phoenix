#pragma once


namespace Phoenix{
	class Texture;
	// class UUID;

	class BasicMaterial{
		public:
			BasicMaterial(glm::vec4 _color = {1.0f, 1.0f, 1.0f, 1.0f}, float _shininess = 32.0f)
				: color(_color), shininess(_shininess) {};
			BasicMaterial(UUID texture, float _shininess)
				: _texture(texture), shininess(_shininess), _using_texture(true) {};
			~BasicMaterial() = default;
	

			void useColor();

			inline bool usingTexture() const { return _using_texture; };
			inline UUID& getTexture() { return _texture; };
			void setTexture(UUID filepath);

		public:
			glm::vec4 color;
			float shininess;

		private:
			UUID _texture;
			bool _using_texture = false;

	};

}