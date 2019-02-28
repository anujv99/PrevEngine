#pragma once

#include "pch.h"
#include "api/shader.h"
#include "api/instancedbuffer.h"

namespace prev {

	class BaseRenderer {
	public:
		//Define this in specific api cpp file
		static void RenderQuad(const Shader * shader, glm::vec2 &position, glm::vec2 &scale, float rotation);
		//The user should manually load uniforms, matrix etc.
		static void RenderQuadInstanced(const InstancedBuffer * buffer);
	};

}