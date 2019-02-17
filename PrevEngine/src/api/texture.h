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

	class TiledTexture {
		friend class TextureManager;
	public:
		inline unsigned short GetTileSizeX() const { return m_TileX; }
		inline unsigned short GetTileSizeY() const { return m_TileY; }

		inline const Texture * GetTexture() const { return m_Texture; }
	private:
		TiledTexture(const Texture * texture, unsigned short tileSizeX, unsigned short tileSizeY) :
			m_Texture(texture), m_TileX(tileSizeX), m_TileY(tileSizeY) {}
	private:
		unsigned short m_TileX, m_TileY;
		const Texture * m_Texture;
	};

}