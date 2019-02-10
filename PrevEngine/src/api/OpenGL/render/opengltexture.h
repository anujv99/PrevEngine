#pragma once

#include <glad/glad.h>
#include "api/texture.h"

namespace prev { namespace opengl {

	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const char * textureFile);
		~OpenGLTexture();

		// Inherited via Texture
		inline virtual void UseTexture(unsigned int textureUnit) const override { 
			glActiveTexture(GL_TEXTURE0 + textureUnit); 
			glBindTexture(GL_TEXTURE_2D, m_TextureID); 
		}

		virtual bool ReadTexture(std::string & textureFile) override;
	private:
		GLuint m_TextureID;
	};

} }