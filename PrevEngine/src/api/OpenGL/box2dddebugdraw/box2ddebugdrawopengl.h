#pragma once

#include "DebugDraw.h"
#include "collision/box2ddebuglayer.h"

namespace prev { namespace opengl {

	class Box2DDebugDrawOpenGL : public Box2DDebugWrapper {
	public:
		Box2DDebugDrawOpenGL();
		~Box2DDebugDrawOpenGL();
		virtual void OnUpdate() override;
		virtual b2Draw * Getb2Draw() override;
	};

} }
