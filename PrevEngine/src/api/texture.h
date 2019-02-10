#pragma once

namespace prev {

	class Texture {
		friend class TextureManager;
	public:
		virtual ~Texture() { }
		virtual void UseTexture(unsigned int textureUnit) const = 0;
		inline bool IsTextureComplete() const { return isComplete; }
	protected:
		Texture(const char * textureFile);
		void LoadTexture();
		virtual bool ReadTexture(std::string & textureFile) = 0;
	protected:
		std::string m_TextureFile;
		int m_TextureWidth, m_TextureHeight;
		int m_TextureChannels;
		bool isComplete = false;
	private:
		static Texture * Create(const char * textureFile);
	};

}