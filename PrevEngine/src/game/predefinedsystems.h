#pragma once

#include "pch.h"
#include "api/shadermanager.h"
#include "api/baserenderer.h"
#include "game/predefinedcomponents.h"

namespace prev { namespace systems {

	struct RenderSystem : entityx::System<RenderSystem> {
		virtual void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override {
			entities.each<components::Renderable>([](entityx::Entity entity, components::Renderable & renderable) -> void {
				TIME_THIS_SCOPE_MS
				glm::vec2 pos = entity.component<components::Position>()->position;
				glm::vec2 scale = entity.component<components::Scale>()->scale;
				const Shader * shader = ShaderManager::GetActiveShader();
				float rotation = 0.0f;
				if (entity.has_component<components::Rotation>()) {
					rotation = entity.component<components::Rotation>()->angle;
				}
				if (entity.has_component<components::TextureComp>()) {
					entity.component<components::TextureComp>()->texture->UseTexture(0);
				}
				BaseRenderer::RenderQuad(shader, pos, scale, rotation);
			});
		}
	};

	struct CollisionSystem : entityx::System<CollisionSystem> {
		virtual void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override {
			entities.each<components::Collision>([](entityx::Entity entity, components::Collision & collidor) -> void {
				auto posHandle = entity.component<components::Position>();
				posHandle->position = VEC2TOGLM(collidor.body->GetPosition());

				if (entity.has_component<components::Rotation>()) {
					entity.component<components::Rotation>()->angle = collidor.body->GetAngle();
				}
			});
		}
	};

} }