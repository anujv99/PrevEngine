#include "pch.h"
#include "animatedsprite.h"

namespace prev {

	AnimatedSprite::AnimatedSprite(std::string & spriteName, std::string & path, std::string & extension, unsigned int numFrames, float frameDelta) {
		for (unsigned int i = 0; i < numFrames; i++) {
			auto texture = TextureManager::LoadTexture((spriteName + std::to_string(i)).c_str(), (path + std::to_string(i) + extension).c_str());
			PV_ASSERT(texture, "Unable to load texture : %s", (path + std::to_string(i) + extension).c_str());
			m_Textures.push_back(texture);
		}
		m_TimeBetweenFrames = frameDelta;
		m_NumberOfFrames = numFrames;
	}

	AnimatedSprite::~AnimatedSprite() {
		m_Textures.clear();
	}

	void AnimatedSprite::Update() {
		m_PassedTime += Timer::GetDeltaTime();
		if (m_PassedTime >= m_TimeBetweenFrames) {
			m_PassedTime = 0.0f;
			m_CurrentFrame++;
		}
		if (m_CurrentFrame >= m_NumberOfFrames) m_CurrentFrame = 0;
	}

}