#pragma once

#include "pch.h"

namespace prev { namespace components {

	struct Position {
		Position(float x = 0.0f, float y = 0.0f) : 
			position(x, y) {}
		glm::vec2 position;
	};

	struct Scale {
		Scale(float x = 1.0f, float y = 1.0f) :
			scale(x, y) {}
		glm::vec2 scale;
	};

	struct Rotation {
		Rotation(float rotation = 0.0f) :
			angle(rotation) {}
		float angle;
	};

	struct Renderabel {
		// TODO
	};

} }