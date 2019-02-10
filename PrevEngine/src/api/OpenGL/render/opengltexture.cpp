#include "pch.h"
#include "opengltexture.h"
#include "stb_image.h"

namespace prev {

	Texture * Texture::Create(const char * textureFile) {
		Texture * texture = new opengl::OpenGLTexture(textureFile);
		texture->LoadTexture();
		return texture;
	}

	namespace opengl {

		OpenGLTexture::OpenGLTexture(const char * textureFile) : 
			Texture(textureFile) {
			glGenTextures(1, &m_TextureID);
			UseTexture(0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		OpenGLTexture::~OpenGLTexture() {
			glDeleteTextures(1, &m_TextureID);
		}

		bool OpenGLTexture::ReadTexture(std::string & textureFile) {
			bool status = true;
			unsigned char * data = stbi_load(textureFile.c_str(), &m_TextureWidth, &m_TextureHeight, &m_TextureChannels, STBI_rgb_alpha);
			if (data) {
				UseTexture(0);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_TextureWidth, m_TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				//glGenerateMipmap(GL_TEXTURE_2D); ?
			} else {
				status = false;
			}
			stbi_image_free(data);
			return status;
		}

	} 
}