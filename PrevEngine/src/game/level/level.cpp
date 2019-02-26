#include "pch.h"
#include "level.h"

#include "application.h"

namespace prev {

	void LevelLayer::Configure() {
		if (!m_IsColliding && !m_IsSharingb2World) return;
		m_Entities.entities.each<components::Collision>([this](entityx::Entity entity, components::Collision &collision) -> void {

			if (collision.isCrated) return;

			auto pos = entity.component<components::Position>()->position;
			auto scale = entity.component<components::Scale>()->scale;

			b2BodyDef def;
			def.type = collision.bodyType;
			def.position.Set(pos.x, pos.y);

			if (entity.has_component<components::Rotation>()) {
				def.angle = glm::radians(entity.component<components::Rotation>()->angle);
			}

			collision.body = m_World->CreateBody(&def);

			b2PolygonShape shape;
			shape.SetAsBox(scale.x / 2.0f, scale.y / 2.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = collision.density;
			fixtureDef.friction = collision.friction;

			collision.body->CreateFixture(&fixtureDef);
			collision.isCrated = true;
		});
	}

	LevelLayer::LevelLayer(bool isColliding, glm::vec2 gravity) {
		m_IsColliding = isColliding;
		if (isColliding) {
			m_World = new b2World(VEC2TOB2(gravity));
			m_World->SetDebugDraw(Application::GetApplicationInstance()->GetB2Draw());
		}
	}

	LevelLayer::LevelLayer(b2World * world) {
		m_IsColliding = false;
		if (world != nullptr) {
			m_IsSharingb2World = true;
			m_World = world;
		}
	}

	LevelLayer::~LevelLayer() {
		if (m_IsColliding) delete m_World;
	}

	void LevelLayer::Update() {
		if (m_IsColliding) {
			m_World->Step(1.0f / 60.0f, 6, 2);
			m_World->DrawDebugData();
		}
		Update(Timer::GetDeltaTime());
		if (m_Shader != nullptr) ShaderManager::ActivateShader(m_Shader);
		m_Entities.Update();
	}

	LevelLayer * Level::CreateNewLevelLayer(bool isColliding, glm::vec2 gravity) {
		LevelLayer * levelLayer = new LevelLayer(isColliding, gravity);
		m_LevelLayers.push_back(levelLayer);
		return levelLayer;
	}

	LevelLayer * Level::CreateNewLevelLayer(b2World * world) {
		LevelLayer * levelLayer = new LevelLayer(world);
		m_LevelLayers.push_back(levelLayer);
		return levelLayer;
	}

	Level::~Level() {
		for (auto levelLayer : m_LevelLayers) {
			delete levelLayer;
		}
		m_LevelLayers.clear();
	}

	void Level::OnUpdate() {
		for (auto levelLayer : m_LevelLayers) {
			levelLayer->Update();
		}
	}

}