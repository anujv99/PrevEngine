#pragma once

namespace prev {

	#define VEC2TOGLM(x) *((glm::vec2*)(&x))
	#define VEC2TOB2(x) *((b2Vec2*)(&x))

}