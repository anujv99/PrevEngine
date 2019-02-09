#pragma once

#include "pch.h"
#include "api/shader.h"

namespace prev {

	class BaseRenderer {
	public:
		//Define this in specific api cpp file
		static void RenderQuad(const Shader * shader, glm::vec2 &position, glm::vec2 &scale, float rotation);
	};

}