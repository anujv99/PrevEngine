#include "pch.h"
#include "level.h"

#include "application.h"

namespace prev {

	void Level::Configure() {
		m_Entities.entities.each<components::Collision>([this](entityx::Entity entity, components::Collision &collision) -> void {
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
		});
	}

	Level::Level(glm::vec2 gravity) {
		m_World = new b2World(VEC2TOB2(gravity));
		m_World->SetDebugDraw(Application::GetApplicationInstance()->GetB2Draw());
	}

	Level::~Level() {
		delete m_World;
	}

	void Level::OnUpdate() {
		m_World->Step(1.0f / 60.0f, 6, 2);
		m_World->DrawDebugData();
		Update(Timer::GetDeltaTime());
		if (m_Shader != nullptr) ShaderManager::ActivateShader(m_Shader);
		m_Entities.Update();
	}

}