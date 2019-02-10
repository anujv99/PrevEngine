#include "pch.h"
#include "texture.h"

namespace prev {

	Texture::Texture(const char * textureFile) {
		m_TextureFile = textureFile;
	}

	void Texture::LoadTexture() { 
		isComplete = ReadTexture(m_TextureFile);
	}

}