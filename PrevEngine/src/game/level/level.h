#pragma once

#include "api/shader.h"
#include "engine/layer.h"
#include "game/entityxlayer.h"

namespace prev {

	class Level : public Layer {
		friend class LevelManager;
	public:
		inline EntityXLayer * GetEntityXLayer() { return &m_Entities; }
		inline void SetShader(const Shader * shader) { m_Shader = shader; }
		inline b2World * GetBox2DWorld() { return m_World; }

		//Call this after setting up level
		void Configure();
	private:
		Level(glm::vec2 gravity = { 0.0f, -9.8f });
		~Level();
	protected:
		virtual void Update(float dt) {}

		const Shader * m_Shader = nullptr;
		b2World * m_World = nullptr;
		EntityXLayer m_Entities;
	private:
		void OnUpdate() override;
	};

}