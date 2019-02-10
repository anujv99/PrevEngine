#include "pch.h"
#include "texturemanager.h"

namespace prev {

	std::map<std::string, Texture *> TextureManager::s_Textures;

	const Texture * TextureManager::LoadTexture(const char * textureName, const char * textureFile) {
		Texture * texture = Texture::Create(textureFile);
		if (!texture->IsTextureComplete()) {
			delete texture;
			PV_CORE_CRITICAL("Unable to load texture : %s", textureName);
			return nullptr;
		}
		std::string stdTextureName = textureName;
		s_Textures.insert(std::pair<std::string, Texture *>(stdTextureName, texture));
		PV_CORE_TRACE("Texture successfully loaded : %s", textureName);
		return s_Textures[stdTextureName];
	}

	void TextureManager::ReleaseTextures() {
		for (auto texture : s_Textures) {
			delete texture.second;
		}
		s_Textures.clear();
	}

}