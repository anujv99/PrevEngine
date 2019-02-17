#pragma once

#include "api/texturemanager.h"

namespace prev {

	class AnimatedSprite {
	public:
		AnimatedSprite(std::string & spriteName, std::string & path, std::string & extension, unsigned int numFrames, float frameDelta);
		~AnimatedSprite();

		void Update();
		inline const Texture * GetActiveTexture() { return m_Textures[m_CurrentFrame]; }
	private:
		std::vector<const Texture *> m_Textures;
		float m_TimeBetweenFrames = 0.0f, m_PassedTime = 0.0f;
		unsigned int m_NumberOfFrames = 0, m_CurrentFrame = 0;
	};

}