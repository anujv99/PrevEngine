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

	struct Renderable {
		Renderable(const Shader * shade) :
			shader(shade) {
		}
		const Shader * shader;
	};

} }