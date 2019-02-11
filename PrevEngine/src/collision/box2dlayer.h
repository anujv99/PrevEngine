#pragma once

#include "engine/layer.h"

namespace prev {

	class Box2DLayer : public Layer {
	public:
		Box2DLayer(glm::vec2 gravity);
		~Box2DLayer();
	private:
		b2World m_World;
	};

}