#pragma once

#include "pch.h"
#include "api/shader.h"
#include "api/texture.h"

namespace prev { namespace components {

	struct Position {
		Position(glm::vec2 pos = { 0, 0 }) :
			position(pos) {}
		glm::vec2 position;
	};

	struct Scale {
		Scale(glm::vec2 scl = { 1, 1 }) :
			scale(scl) {}
		glm::vec2 scale;
	};

	struct Rotation {
		Rotation(float rotation = 0.0f) :
			angle(rotation) {}
		float angle;
	};

	struct TextureComp {
		TextureComp(const Texture * tex) :
			texture(tex) {}
		const Texture * texture;
	};

	struct TiledTextureComp {
		TiledTextureComp(const TiledTexture * tex, unsigned short tile) :
			texture(tex), tileToUse(tile) {
		}
		const TiledTexture * texture;
		unsigned short tileToUse;
	};

	struct Renderable {
		Renderable() {}
	};

	struct Collision {
		Collision(b2BodyType type, float fric, float dense) {
			bodyType = type;
			friction = fric;
			density = dense;
		}
		b2Body * body = nullptr;
		float friction, density;
		b2BodyType bodyType;
	};

} }