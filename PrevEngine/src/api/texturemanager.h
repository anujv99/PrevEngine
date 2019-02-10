#pragma once

#include "texture.h"

namespace prev {

	class TextureManager {
	public:
		static const Texture * LoadTexture(const char * textureName, const char * textureFile);
		static void ReleaseTextures();
	private:
		static std::map<std::string, Texture *> s_Textures;
	};

}