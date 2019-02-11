#include "pch.h"
#include "box2dlayer.h"

#define GLMTOB2(x) *((b2Vec2*)(&x))

namespace prev {

	Box2DLayer::Box2DLayer(glm::vec2 gravity) : 
		m_World(GLMTOB2(gravity)) {
	}

	Box2DLayer::~Box2DLayer() {
	}

}