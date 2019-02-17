#pragma once

#include "texture.h"

namespace prev {

	class TextureManager {
	public:
		static const Texture * LoadTexture(const char * textureName, const char * textureFile);
		static const TiledTexture * LoadTiledTexture(const char * textureName, const char * textureFile, unsigned short tileSizeX, unsigned short tileSizeY);
		static void ReleaseTextures();
	private:
		static std::map<std::string, Texture *> s_Textures;
		static std::map<std::string, TiledTexture *> s_TiledTextures;
	};

}