#pragma once

#include "pch.h"
#include "api/shader.h"
#include "api/baserenderer.h"
#include "game/predefinedcomponents.h"

namespace prev { namespace systems {

	struct RenderSystem : entityx::System<RenderSystem> {
		virtual void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override {
			entities.each<components::Renderable>([](entityx::Entity entity, components::Renderable & renderable) -> void {
				glm::vec2 pos = entity.component<components::Position>()->position;
				glm::vec2 scale = entity.component<components::Scale>()->scale;
				const Shader * shader = entity.component<components::Renderable>()->shader;
				float rotation = 0.0f;
				if (entity.has_component<components::Rotation>()) {
					rotation = entity.component<components::Rotation>()->angle;
				}
				if (entity.has_component<components::TextureComp>()) {
					entity.component<components::TextureComp>()->texture->UseTexture(0);
				}
				BaseRenderer::RenderQuad(renderable.shader, pos, scale, rotation);
			});
		}
	};



} }