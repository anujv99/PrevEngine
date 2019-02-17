#include "pch.h"
#include "texturemanager.h"

namespace prev {

	std::map<std::string, Texture *> TextureManager::s_Textures;
	std::map<std::string, TiledTexture *> TextureManager::s_TiledTextures;

	const Texture * TextureManager::LoadTexture(const char * textureName, const char * textureFile) {
		if (s_Textures.find(textureName) != s_Textures.end()) {
			PV_ASSERT(false, "Texture with name (%s) already exists", textureName);
			return nullptr;
		}
		Texture * texture = Texture::Create(textureFile);
		if (!texture->IsTextureComplete()) {
			delete texture;
			PV_ASSERT(false, "Unable to load texture : %s", textureName);
			return nullptr;
		}
		std::string stdTextureName = textureName;
		s_Textures.insert(std::pair<std::string, Texture *>(stdTextureName, texture));
		PV_CORE_TRACE("Texture successfully loaded : %s", textureName);
		return s_Textures[stdTextureName];
	}

	const TiledTexture * TextureManager::LoadTiledTexture(const char * textureName, const char * textureFile, unsigned short tileSizeX, unsigned short tileSizeY) {
		const Texture * texture = LoadTexture(textureName, textureFile);
		TiledTexture * tiledTexture = new TiledTexture(texture, tileSizeX, tileSizeY);
		s_TiledTextures.insert(std::pair<std::string, TiledTexture *>(std::string(textureName), tiledTexture));
		return s_TiledTextures[std::string(textureName)];
	}

	void TextureManager::ReleaseTextures() {
		for (auto tiledTexture : s_TiledTextures) {
			delete tiledTexture.second;
		}
		s_TiledTextures.clear();
		for (auto texture : s_Textures) {
			delete texture.second;
		}
		s_Textures.clear();
	}

}