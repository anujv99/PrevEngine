#pragma once

#include "api/shader.h"
#include "engine/layer.h"
#include "game/entityxlayer.h"

namespace prev {

	class LevelLayer {
		friend class Level;
	public:
		inline EntityXLayer * GetEntityXLayer() { return &m_Entities; }
		inline void SetShader(const Shader * shader) { m_Shader = shader; }
		inline b2World * GetBox2DWorld() { return m_World; }

		//Must Call this after setting up level
		void Configure();
	private:
		LevelLayer(bool isColliding, glm::vec2 gravity);
		LevelLayer(b2World * world);
		~LevelLayer();
	protected:
		virtual void Update(float dt) {}

		const Shader * m_Shader = nullptr;
		b2World * m_World = nullptr;
		EntityXLayer m_Entities;
		bool m_IsColliding = false;
		bool m_IsSharingb2World = false;
	private:
		void Update();
	};

	class Level : public Layer {
		friend class LevelManager;
	public:
		LevelLayer * CreateNewLevelLayer(bool isColliding, glm::vec2 gravity = { 0.0f, -9.8f });
		LevelLayer * CreateNewLevelLayer(b2World * world);
	private:
		~Level();

		virtual void OnUpdate() override;
		std::vector<LevelLayer *> m_LevelLayers;
	};

}